#include "sequential.h"
#include "simulator.h"
#include "event.h"
#include <stdexcept>

uint32_t DFF::id_counter = 0;

// ===== SequentialElement Base Class =====

SequentialElement::SequentialElement(const std::string& element_id, uint64_t delay, Edge edge)
    : trigger_edge(edge), clock(nullptr), last_clock_value(2) {  // 2 = unknown initially
    id = element_id;
    propagation_delay = delay;
}

void SequentialElement::connect_clock(Signal* clk) {
    if (!clk) {
        throw std::invalid_argument("Cannot connect null clock signal");
    }
    clock = clk;
    last_clock_value = clk->get_value();  // Initialize to current value
    clk->attach_observer(this);
}

bool SequentialElement::detect_edge() {
    if (!clock) {
        return false;
    }
    
    uint8_t current_clock = clock->get_value();
    bool edge_detected = false;
    
    switch (trigger_edge) {
        case RISING:
            edge_detected = (last_clock_value == 0 && current_clock == 1);
            break;
        case FALLING:
            edge_detected = (last_clock_value == 1 && current_clock == 0);
            break;
        case BOTH:
            edge_detected = (last_clock_value != current_clock) && (current_clock != 2);
            break;
    }
    
    last_clock_value = current_clock;
    return edge_detected;
}

void SequentialElement::evaluate(Simulator* sim, uint64_t current_time) {
    if (detect_edge()) {
        on_clock_edge(sim, current_time);
    }
}


// ===== D Flip-Flop Implementation =====

DFF::DFF(uint64_t delay, Edge edge)
    : SequentialElement("DFF" + std::to_string(id_counter++), delay, edge),
      d(nullptr), q(nullptr), async_reset(nullptr), enable(nullptr) {
}

void DFF::connect_data(Signal* data) {
    if (!data) {
        throw std::invalid_argument("Cannot connect null data signal");
    }
    d = data;
    d->attach_observer(this);  // Monitor data changes (for future setup/hold checks)
}

void DFF::connect_q(Signal* output_signal) {
    if (!output_signal) {
        throw std::invalid_argument("Cannot connect null output signal");
    }
    q = output_signal;
    output = output_signal;  // Store in Component base class too
}

void DFF::connect_reset(Signal* rst) {
    if (rst) {
        async_reset = rst;
        async_reset->attach_observer(this);
    }
}

void DFF::connect_enable(Signal* en) {
    if (en) {
        enable = en;
        enable->attach_observer(this);
    }
}

void DFF::on_clock_edge(Simulator* sim, uint64_t current_time) {
    if (!d || !q) {
        return;  // Not fully connected
    }
    
    // Check enable signal (if present, must be high to capture)
    if (enable && enable->get_value() == 0) {
        return;  // Disabled, don't capture
    }
    
    // Sample data input
    uint8_t sampled_value = d->get_value();
    
    // Only schedule event if value changed
    if (sampled_value != q->get_value()) {
        sim->schedule_event(Event(current_time + propagation_delay, q->get_id(), sampled_value));
    }
}

void DFF::evaluate(Simulator* sim, uint64_t current_time) {
    // Handle asynchronous reset (overrides clock)
    if (async_reset && async_reset->get_value() == 1) {
        if (q && q->get_value() != 0) {
            sim->schedule_event(Event(current_time + propagation_delay, q->get_id(), 0));
        }
        return;
    }
    
    // Otherwise, normal sequential evaluation (check for clock edge)
    SequentialElement::evaluate(sim, current_time);
}

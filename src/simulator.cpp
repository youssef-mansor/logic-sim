#include "simulator.h"
#include <stdexcept>
#include <iostream>

Simulator::Simulator() : current_time(0) {}

void Simulator::add_signal(Signal* sig) {
    if (!sig) {
        throw std::invalid_argument("Cannot add null signal");
    }
    
    // Check name uniqueness
    if (signal_by_name.count(sig->get_name())) {
        throw std::runtime_error("Signal name '" + sig->get_name() + "' already exists");
    }
    
    signals.push_back(sig);
    signal_by_name[sig->get_name()] = sig;
    signal_by_id[sig->get_id()] = sig;
}

void Simulator::add_gate(Gate* gate) {
    if (!gate) {
        throw std::invalid_argument("Cannot add null gate");
    }
    gates.push_back(gate);
}

Signal* Simulator::get_signal_by_name(const std::string& name) {
    auto it = signal_by_name.find(name);
    if (it == signal_by_name.end()) {
        return nullptr;
    }
    return it->second;
}

Signal* Simulator::get_signal_by_id(int id) {
    auto it = signal_by_id.find(id);
    if (it == signal_by_id.end()) {
        return nullptr;
    }
    return it->second;
}

void Simulator::schedule_event(const Event& e) {
    event_queue.schedule(e);
}

void Simulator::step() {
    if (event_queue.empty()) {
        return;
    }
    
    // Pop next event
    Event e = event_queue.pop_next();
    current_time = e.time;
    
    // Find the signal
    Signal* sig = get_signal_by_id(e.signal_id);
    if (!sig) {
        throw std::runtime_error("Event references unknown signal ID: " + 
                                 std::to_string(e.signal_id));
    }
    
    // Update signal value
    uint8_t old_value = sig->get_value();
    sig->set_value(e.new_value);
    
    // Debug output (optional, remove later)
    std::cout << "t=" << current_time << ": " << sig->get_name() 
              << " changed from " << (int)old_value << " to " << (int)e.new_value << "\n";
    
    // Notify all gates observing this signal
    for (Gate* gate : sig->get_observers()) {
        gate->evaluate(this, current_time);
    }
}

void Simulator::run_until(uint64_t end_time) {
    while (!event_queue.empty() && event_queue.next_time() <= end_time) {
        step();
    }
}

void Simulator::run_all() {
    while (!event_queue.empty()) {
        step();
    }
}

uint64_t Simulator::get_current_time() const {
    return current_time;
}

Simulator::~Simulator() {
    // Note: We don't delete signals/gates here
    // Assume caller manages memory (or use smart pointers later)
}

#include "gate.h"
#include "event.h"
#include "simulator.h"
#include <stdexcept>

uint32_t ANDGate::id_counter = 0;
uint32_t ORGate::id_counter = 0;
uint32_t NOTGate::id_counter = 0;
uint32_t XORGate::id_counter = 0;

Gate::Gate(std::string gate_id, uint64_t delay) {
    id = gate_id;
    propagation_delay = delay;
}

void Gate::connect_input(Signal* sig) {
    inputs.push_back(sig);
     sig->attach_observer(this);
}

void Gate::connect_output(Signal* sig) {
    output = sig;
}

ANDGate::ANDGate(uint64_t delay) 
    : Gate("AND" + std::to_string(id_counter++), delay){
}

void ANDGate::evaluate(Simulator* sim, uint64_t current_time) {
    if (inputs.size() < 2) return;
    
    uint8_t result = 1; // Start with true
    for (const auto& input : inputs) {
        if (input->get_value() == 0) {
            result = 0; // AND with false yields false
            break;
        } else if (input->get_value() == 2) {
            result = 2; // Unknown if any input is unknown
        }
    }
    
    if (result != output->get_value()) {
        // Schedule event in simulator
        sim->schedule_event(Event(current_time + propagation_delay, output->get_id(), result));
    }

    
}

ORGate::ORGate(uint64_t delay) : Gate("OR" + std::to_string(id_counter++), delay) {
}

void ORGate::evaluate(Simulator* sim, uint64_t current_time) {
    if (inputs.size() < 2) return;

    uint8_t result = 0; // Start with false
    for (const auto& input : inputs) {
        if (input->get_value() == 1) {
            result = 1; // OR with true yields true
            break;
        } else if (input->get_value() == 2) {
            result = 2; // Unknown if any input is unknown
            // TODO: do real simulator evaluate 1 OR X to 1 ?
        }
    }
    if (result != output->get_value()) {
        // Schedule event in simulator
        sim->schedule_event(Event(current_time + propagation_delay, output->get_id(), result));
    }
}

NOTGate::NOTGate(uint64_t delay) : Gate("NOT" + std::to_string(id_counter++), delay) {
}

void NOTGate::evaluate(Simulator* sim, uint64_t current_time) {
    if (inputs.empty()) return;

    uint8_t input_value = inputs[0]->get_value();
    uint8_t result = (input_value == 0) ? 1 : 0; // NOT operation

    if (result != output->get_value()) {
        // Schedule event in simulator
        sim->schedule_event(Event(current_time + propagation_delay, output->get_id(), result));
    }
}

XORGate::XORGate(uint64_t delay): Gate("XOR" + std::to_string(id_counter++), delay) {
}

void XORGate::evaluate(Simulator* sim, uint64_t current_time){
   if (inputs.size() < 2) return;

    // Check for unknown values first
    for (const auto& input: inputs){
        if(input->get_value() == 2){
            if(output->get_value() != 2){
                sim->schedule_event(Event(
                    current_time + propagation_delay,
                    output->get_id(),
                    2
                ));
            }
        }
    }

    uint8_t result = 0;
    for(const auto& input: inputs){
        result ^= input->get_value();
    }

    if (result != output->get_value()) {
        // Schedule event in simulator
        sim->schedule_event(Event(current_time + propagation_delay, output->get_id(), result));
    }
}
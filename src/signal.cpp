#include "signal.h"
#include <stdexcept>

uint32_t Signal::id_counter = 0;

Signal::Signal(const std::string& signal_name, uint8_t initial_value) 
    : id(id_counter++), name(signal_name), current_value(initial_value) {
    /*Initial value validation*/

    // raise error for invalid values
    if (initial_value > 2) {
        throw std::invalid_argument("Initial signal value must be 0, 1, or 2 (for 'X')");
    }
    // raise error for empty names
    if (signal_name.empty()) {
        throw std::invalid_argument("Signal name cannot be empty");
    }
}

uint8_t Signal::get_value() const {
    return current_value;
}

void Signal::set_value(uint8_t new_val) {
    // Value validation, raise error for invalid values
    if (new_val > 2) {
        throw std::invalid_argument("Signal value must be 0, 1, or 2 (for 'X')");
    }   
    current_value = new_val;
}

const std::string& Signal::get_name() const {
    return name;
}

void Signal::attach_observer(Gate* gate) {
    observers.push_back(gate);
}

const std::vector<Gate*>& Signal::get_observers() const {
    return observers;
}

std::string Signal::value_to_string() const {
    switch (current_value) {
        case 0: return "0";
        case 1: return "1";
        case 2: return "X";
        default: return "X";
    }
}

std::string Signal::to_string() const {
    return "Signal " + name + ": " + value_to_string();
}

uint32_t Signal::get_id() const {
    return id;
}
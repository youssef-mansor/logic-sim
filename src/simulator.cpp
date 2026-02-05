#include "simulator.h"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <functional>

// Helper: convert value to char
static char value_to_char(uint8_t val) {
    if (val == 0) return '0';
    if (val == 1) return '1';
    return 'X';
}

Simulator::Simulator() : current_time(0), trace_enabled(false) {
    trace_log.reserve(10000);  // Pre-allocate for performance
}

Signal* Simulator::create_signal(const std::string& name, uint8_t value){
    Signal* sig = new Signal(name, value);
    add_signal(sig);
    return sig;
}

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

    initial_values[sig->get_id()] = sig->get_value();
}

void Simulator::add_component(Component* component) {
    if (!component) {
        throw std::invalid_argument("Cannot add null component");
    }
    components.push_back(component);
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

    std::vector<std::reference_wrapper<const std::vector<Component *>>> observer_lists;
    bool same_step = true;

    while(same_step){
        same_step = false;

        Event e = event_queue.pop_next();
        current_time = e.time;

        // True if multiple signals change at the same time stamp
        // Register these simultaneous changes in one step
        if (!event_queue.empty())
            same_step = event_queue.next_time() == current_time;
        
        Signal* sig = get_signal_by_id(e.signal_id);
        if (!sig) {
            throw std::runtime_error("Event references unknown signal ID: " + 
                                    std::to_string(e.signal_id));
        }
        
        uint8_t old_value = sig->get_value();
        sig->set_value(e.new_value);
        
        observer_lists.push_back(std::cref(sig->get_observers()));

        // NEW: Log the change if tracing
        if (trace_enabled && old_value != e.new_value) {
            trace_log.push_back({current_time, sig->get_name(), old_value, e.new_value});
        }
        
        // Console trace (if enabled)
        if (trace_enabled) {
            std::cout << "t=" << current_time << "ps: " << sig->get_name() 
                    << " " << value_to_char(old_value) << " -> " 
                    << value_to_char(e.new_value) << "\n";
        }
        
    }
    
    // Notify observers
    for (const auto& list_wrapper : observer_lists) {
        
        const std::vector<Component*>& observer_list = list_wrapper.get();
        
        for (Component* component : observer_list) {
            component->evaluate(this, current_time);
        }
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

void Simulator::enable_trace() {
    trace_enabled = true;
    trace_log.clear();
}

void Simulator::disable_trace() {
    trace_enabled = false;
}

void Simulator::print_trace() {
    std::cout << "\n=== Signal Trace ===\n";
    std::cout << std::setw(10) << "Time(ps)" << " | " 
              << std::setw(15) << "Signal" << " | " 
              << "Change\n";
    std::cout << std::string(45, '-') << "\n";
    
    for (const auto& change : trace_log) {
        std::cout << std::setw(10) << change.time << " | "
                  << std::setw(15) << change.signal_name << " | "
                  << value_to_char(change.old_value) << " -> " 
                  << value_to_char(change.new_value) << "\n";
    }
}

void Simulator::dump_waveform(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    // Header
    file << "$date\n";
    file << "  Digital Logic Simulator\n";
    file << "$end\n";
    file << "$timescale 1ps $end\n";
    
    // Signal declarations
    file << "$scope module top $end\n";
    for (const auto& sig : signals) {
        file << "$var wire 1 " << sig->get_id() << " " 
             << sig->get_name() << " $end\n";
    }
    file << "$upscope $end\n";
    file << "$enddefinitions $end\n";
    
    // Initial values
    file << "\n$dumpvars\n";
    for (const auto& sig : signals) {
        char val = value_to_char(initial_values[sig->get_id()]);  // ✓ Use initial
        file << val << sig->get_id() << "\n";
    }
    file << "$end\n";
    
    // Value changes (group by time)
    uint64_t last_time = 0;
    bool first_change = true;
    
    for (const auto& change : trace_log) {
        // Only print timestamp if time changed
        if (first_change || change.time != last_time) {
            file << "#" << change.time << "\n";
            last_time = change.time;
            first_change = false;
        }
        
        Signal* sig = get_signal_by_name(change.signal_name);
        char val = value_to_char(change.new_value);
        file << val << sig->get_id() << "\n";
    }
    
    if (!trace_log.empty()) {
        file << "#" << (last_time + 100) << "\n";
    }

    file.close();
    std::cout << "Waveform dumped to: " << filename << "\n";
}


Simulator::~Simulator() {
    // Note: We don't delete signals/gates here
    // Assume caller manages memory (or use smart pointers later)
}

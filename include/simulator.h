#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "event_queue.h"
#include "signal.h"
#include "component.h"
#include <vector>
#include <map>
#include <string>
#include <cstdint>

class Simulator {
private:
    EventQueue event_queue;
    std::vector<Signal*> signals;
    std::vector<Component*> components;
    std::map<std::string, Signal*> signal_by_name;
    std::map<int, Signal*> signal_by_id;
    uint64_t current_time;
    bool trace_enabled;
    struct SignalChange {
        uint64_t time;
        std::string signal_name;
        uint8_t old_value;
        uint8_t new_value;
    };
    std::vector<SignalChange> trace_log;
    std::map<int, uint8_t> initial_values; // for vcd dump
    
public:
    Simulator();
    
    // Component management
    Signal* create_signal(const std::string& name, uint8_t value); // Create and register signal

    // Create and register components
    template<typename ComponentType>
    ComponentType* create_component(uint64_t delay){
        ComponentType* component = new ComponentType(delay);
        add_component(component);
        return component;
    }
    void add_signal(Signal* sig);
    void add_component(Component* component);
    
    // Signal lookup
    Signal* get_signal_by_name(const std::string& name);
    Signal* get_signal_by_id(int id);
    
    // Event scheduling (called by gates)
    void schedule_event(const Event& e);
    
    // Simulation control
    void step();                          // Process one event
    void run_until(uint64_t end_time);   // Run until time limit
    void run_all();                      // Run until queue empty
    
    // Time access
    uint64_t get_current_time() const;

    // Waveform output
    void enable_trace();
    void disable_trace();
    void dump_waveform(const std::string& filename);
    void print_trace();
    
    // Cleanup
    ~Simulator();
};

#endif

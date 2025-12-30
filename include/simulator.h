#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "event_queue.h"
#include "signal.h"
#include "gate.h"
#include <vector>
#include <map>
#include <string>
#include <cstdint>

class Simulator {
private:
    EventQueue event_queue;
    std::vector<Signal*> signals;
    std::vector<Gate*> gates;
    std::map<std::string, Signal*> signal_by_name;
    std::map<int, Signal*> signal_by_id;
    uint64_t current_time;
    
public:
    Simulator();
    
    // Component management
    void add_signal(Signal* sig);
    void add_gate(Gate* gate);
    
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
    
    // Cleanup
    ~Simulator();
};

#endif

#ifndef SIGNAL_H
#define SIGNAL_H

#include <string>
#include <vector>
#include <cstdint>

// Forward declaration for observer pattern (Sprint 7)
class Gate;

class Signal {
private:
    static uint32_t id_counter;  // Static counter for unique IDs
    uint32_t id;  // Unique identifier for the signal
    std::string name;
    uint8_t current_value;  // 0, 1, or 2 (for 'X' unknown)
    std::vector<Gate*> observers;  // Gates that depend on this signal
    
public:
    // Constructor
    explicit Signal(const std::string& signal_name, uint8_t initial_value = 2);
    
    // Value/ID access (encapsulation)
    uint8_t get_value() const;
    void set_value(uint8_t new_val);
    uint32_t get_id() const;
    
    // Identity
    const std::string& get_name() const;
    
    // Observer management (for future sprints)
    void attach_observer(Gate* gate);
    const std::vector<Gate*>& get_observers() const;
    
    // Utility
    std::string value_to_string() const;  // "0", "1", or "X"
    std::string to_string() const;
};

#endif

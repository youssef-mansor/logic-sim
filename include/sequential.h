#ifndef SEQUENTIAL_H
#define SEQUENTIAL_H

#include "component.h"
#include "signal.h"
#include <cstdint>
#include <string>

class Simulator;  // Forward declaration

// Base class for sequential (stateful) elements
class SequentialElement : public Component {
protected:
    Signal* clock;
    uint8_t last_clock_value;
    
    enum Edge {
        RISING,
        FALLING,
        BOTH
    };
    
    Edge trigger_edge;
    
    // Detect clock edge and update last_clock_value
    bool detect_edge();
    
    // Called when trigger edge is detected
    virtual void on_clock_edge(Simulator* sim, uint64_t current_time) = 0;
    
public:
    SequentialElement(const std::string& element_id, uint64_t delay, Edge edge = RISING);
    virtual ~SequentialElement() = default;
    
    // Connect clock signal
    void connect_clock(Signal* clk);
    
    // Evaluate checks for clock edge and calls on_clock_edge
    void evaluate(Simulator* sim, uint64_t current_time) override;
};


// D Flip-Flop (positive edge triggered)
class DFF : public SequentialElement {
private:
    static uint32_t id_counter;
    Signal* d;           // Data input
    Signal* q;           // Output
    Signal* async_reset; // Asynchronous reset (optional)
    Signal* enable;      // Enable signal (optional)
    
protected:
    void on_clock_edge(Simulator* sim, uint64_t current_time) override;
    
public:
    DFF(uint64_t delay = 100, Edge edge = RISING);
    
    // Connect signals
    void connect_data(Signal* data);
    void connect_q(Signal* output);
    void connect_reset(Signal* rst);
    void connect_enable(Signal* en);
    
    // Override evaluate to handle async reset
    void evaluate(Simulator* sim, uint64_t current_time) override;
};

#endif // SEQUENTIAL_H

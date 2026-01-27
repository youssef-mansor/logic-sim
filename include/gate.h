#ifndef GATE_H
#define GATE_H

#include <vector>
#include <cstdint>
#include "signal.h"

class Simulator;  // Forward declaration

class Gate {
protected:
    std::vector<Signal*> inputs;
    Signal* output;
    uint64_t propagation_delay;  // picoseconds
    
public:
    virtual ~Gate() = default;
    
    // Pure virtual - subclasses define logic
    virtual void evaluate(Simulator* sim, uint64_t current_time) = 0;
    
    // Common functionality
    void connect_input(Signal* sig);
    void connect_output(Signal* sig);
    uint64_t get_delay() const;
};

class ANDGate : public Gate {
public:
    ANDGate(uint64_t delay = 100);
    void evaluate(Simulator* sim, uint64_t current_time) override;
};

class ORGate : public Gate {
public:
    ORGate(uint64_t delay = 100);
    void evaluate(Simulator* sim, uint64_t current_time) override;
};

class NOTGate : public Gate {
public:
    NOTGate(uint64_t delay = 50);
    void evaluate(Simulator* sim, uint64_t current_time) override;
};

// TODO: verify by AI. What should be the delay?
class XORGate : public Gate {
public:
    XORGate(uint64_t delay = 50);
    void evaluate(Simulator* sim, uint64_t current_time) override;
};

#endif // GATE_H
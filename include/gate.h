#ifndef GATE_H
#define GATE_H

#include <vector>
#include <cstdint>
#include "signal.h"
#include "component.h"

class Simulator;  // Forward declaration

class Gate : public Component {
public:
    Gate(std::string id, uint64_t delay);
    virtual ~Gate() = default;

    // Common functionality
    void connect_input(Signal* sig);
    void connect_output(Signal* sig);
};

class ANDGate : public Gate {
private:
    static uint32_t id_counter;
public:
    ANDGate(uint64_t delay = 100);
    void evaluate(Simulator* sim, uint64_t current_time) override;
};

class ORGate : public Gate {
private:
    static uint32_t id_counter;
public:
    ORGate(uint64_t delay = 100);
    void evaluate(Simulator* sim, uint64_t current_time) override;
};

class NOTGate : public Gate {
private:
    static uint32_t id_counter;
public:
    NOTGate(uint64_t delay = 50);
    void evaluate(Simulator* sim, uint64_t current_time) override;
};

class XORGate : public Gate {
private:
    static uint32_t id_counter;
public:
    XORGate(uint64_t delay = 50);
    void evaluate(Simulator* sim, uint64_t current_time) override;
};

#endif // GATE_H
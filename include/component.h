#ifndef COMPONENT_H
#define COMPONENT_H

#include <vector>
#include <cstdint>
#include <string>
#include "signal.h"

class Simulator;  // Forward declaration

class Component{
protected:
    std::string id;
    uint64_t propagation_delay;
    std::vector<Signal*> inputs;
    Signal* output;
public:
    virtual void evaluate(Simulator* sim, uint64_t current_time) = 0;
    virtual ~Component() = default;
    uint64_t get_delay() const;
    std::string get_id() const;
};

#endif // COMPONENT_H
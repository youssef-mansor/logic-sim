# Digital Logic Simulator

⚠️ **Work in Progress** - Basic event-driven logic simulator in C++.

## Build & Run

```bash
mkdir build && cd build
cmake ..
make
./test_integration
```

## Quick Start

```cpp
#include "simulator.h"
#include "signal.h"
#include "gate.h"

int main() {
    Simulator sim;
    
    // Create signals
    Signal* a = new Signal("A", 0);
    Signal* b = new Signal("B", 0);
    Signal* y = new Signal("Y", 2);
    
    sim.add_signal(a);
    sim.add_signal(b);
    sim.add_signal(y);
    
    // Create AND gate
    ANDGate* gate = new ANDGate(100);  // 100ps delay
    gate->connect_input(a);
    gate->connect_input(b);
    gate->connect_output(y);
    sim.add_gate(gate);
    
    // Schedule inputs
    sim.schedule_event(Event(0, a->get_id(), 1));
    sim.schedule_event(Event(0, b->get_id(), 1));
    
    // Run
    sim.run_until(200);
    
    std::cout << "Y = " << (int)y->get_value() << "\n";  // Output: 1
    
    return 0;
}
```

## Available Gates

- `ANDGate(delay)` - AND logic
- `ORGate(delay)` - OR logic  
- `NOTGate(delay)` - NOT logic

## Status

Currently implemented: Events, Signals, Basic Gates, Simulator kernel  
Coming soon: Sequential elements, netlist parser, waveform output
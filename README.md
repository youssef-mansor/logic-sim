# Digital Logic Simulator

⚠️ **Work in Progress** - Event-driven digital logic simulator in C++.

## Build & Run

```bash
mkdir build && cd build
cmake ..
make
./test_integration
```

## Example: Half-Adder Circuit

```cpp
#include "simulator.h"
#include "signal.h"
#include "gate.h"

int main() {
    Simulator sim;
    sim.enable_trace();
    
    // Create signals
    Signal* a = sim.create_signal("A", 0);
    Signal* b = sim.create_signal("B", 0);
    Signal* sum = sim.create_signal("Sum", 2);
    Signal* carry = sim.create_signal("Carry", 2);
    
    // Build half adder: Sum = A XOR B, Carry = A AND B
    XORGate* xor_gate = sim.create_gate<XORGate>(100);
    xor_gate->connect_input(a);
    xor_gate->connect_input(b);
    xor_gate->connect_output(sum);
    
    ANDGate* and_gate = sim.create_gate<ANDGate>(100);
    and_gate->connect_input(a);
    and_gate->connect_input(b);
    and_gate->connect_output(carry);
    
    // Test case 1: 0 + 0 = 00
    std::cout << "\nTest 1: A=0, B=0 -> Sum=0, Carry=0\n";
    sim.schedule_event(Event(0, a->get_id(), 0));
    sim.schedule_event(Event(0, b->get_id(), 0));
    sim.run_until(200);
    assert(sum->get_value() == 0);
    assert(carry->get_value() == 0);
    std::cout << "✓ Passed\n";
    
    // Test case 2: 0 + 1 = 01
    
    // Test case 3: 1 + 0 = 01
    
    // Test case 4: 1 + 1 = 10
    
    // Output trace
    std::cout << "\n";
    sim.print_trace();
    sim.dump_waveform("half_adder.vcd");
    
    return 0;
}
```
Note: Full test found in tests/test_comp.cpp

### Waveform Output

![Half-Adder Simulation](images/Half-adder-sim.png)

Note: Waveforms include propagation delay.

View VCD files with [GTKWave](http://gtkwave.sourceforge.net/) or any waveform viewer.

## Available Gates

- `ANDGate(delay)` - AND logic
- `ORGate(delay)` - OR logic  
- `NOTGate(delay)` - NOT logic
- `XORGate(delay)` - XOR logic

## Features

✅ Event-driven simulation with configurable gate delays  
✅ Multi-gate circuits (half-adder, full-adder tested)  
✅ VCD waveform output for visualization  
✅ Signal tracing and debug output  

## Status

**Currently implemented:** Events, Signals, Basic Gates, Multi-gate circuits, Waveform tracing  
**Coming soon:** Sequential elements (flip-flops), Netlist parser, Memory optimization
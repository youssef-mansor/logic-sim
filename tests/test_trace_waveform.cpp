#include "simulator.h"
#include "signal.h"
#include "gate.h"
#include "event.h"
#include <iostream>
#include <cassert>

int main() {
    Simulator sim;
    sim.enable_trace();  // Enable console output
    
    // Build circuit...
    Signal* a = sim.create_signal("A", 0);
    Signal* b = sim.create_signal("B", 0);
    Signal* y = sim.create_signal("Y", 2);
    
    ANDGate* gate = sim.create_gate<ANDGate>(100);
    gate->connect_input(a);
    gate->connect_input(b);
    gate->connect_output(y);
    
    // Run simulation
    sim.schedule_event(Event(0, a->get_id(), 1));
    sim.schedule_event(Event(50, b->get_id(), 1));
    sim.run_until(300);
    
    // Output results
    sim.print_trace();
    sim.dump_waveform("output.vcd");
    
    return 0;
}

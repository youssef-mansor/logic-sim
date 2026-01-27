#include "simulator.h"
#include "signal.h"
#include "gate.h"
#include "event.h"
#include <iostream>
#include <cassert>

void test_and_gate_basic() {
    std::cout << "\n=== Test: AND Gate Basic Logic ===\n";
    
    Simulator sim;
    
    // Create signals
    Signal* a = new Signal("A", 0);
    Signal* b = new Signal("B", 0);
    Signal* y = new Signal("Y", 2);  // Unknown initially
    
    sim.add_signal(a);
    sim.add_signal(b);
    sim.add_signal(y);
    
    // Create AND gate with 100ps delay
    ANDGate* and_gate = new ANDGate(100);
    and_gate->connect_input(a);
    and_gate->connect_input(b);
    and_gate->connect_output(y);
    sim.add_gate(and_gate);
    
    // Test case 1: A=0, B=0 -> Y should be 0
    std::cout << "\nTest 1: A=0, B=0\n";
    sim.schedule_event(Event(0, a->get_id(), 0));
    sim.schedule_event(Event(0, b->get_id(), 0));
    sim.run_until(100);
    assert(y->get_value() == 0);
    std::cout << "✓ Output Y = 0 (correct)\n";
    
    // Test case 2: A=1, B=0 -> Y should be 0
    std::cout << "\nTest 2: A=1, B=0\n";
    sim.schedule_event(Event(100, a->get_id(), 1));
    sim.run_until(200);
    assert(y->get_value() == 0);
    std::cout << "✓ Output Y = 0 (correct)\n";
    
    // Test case 3: A=1, B=1 -> Y should be 1
    std::cout << "\nTest 3: A=1, B=1\n";
    sim.schedule_event(Event(200, b->get_id(), 1));
    sim.run_until(300);
    assert(y->get_value() == 1);
    std::cout << "✓ Output Y = 1 (correct)\n";
    
    // Test case 4: A=0, B=1 -> Y should be 0
    std::cout << "\nTest 4: A=0, B=1\n";
    sim.schedule_event(Event(300, a->get_id(), 0));
    sim.run_until(400);
    assert(y->get_value() == 0);
    std::cout << "✓ Output Y = 0 (correct)\n";
    
    // Cleanup
    delete a;
    delete b;
    delete y;
    delete and_gate;
    
    std::cout << "\n✓ AND gate test passed!\n";
}

void test_or_gate_basic() {
    std::cout << "\n=== Test: OR Gate Basic Logic ===\n";
    
    Simulator sim;
    
    Signal* a = new Signal("A", 0);
    Signal* b = new Signal("B", 0);
    Signal* y = new Signal("Y", 2);
    
    sim.add_signal(a);
    sim.add_signal(b);
    sim.add_signal(y);
    
    ORGate* or_gate = new ORGate(100);
    or_gate->connect_input(a);
    or_gate->connect_input(b);
    or_gate->connect_output(y);
    sim.add_gate(or_gate);
    
    // A=0, B=0 -> Y=0
    sim.schedule_event(Event(0, a->get_id(), 0));
    sim.schedule_event(Event(0, b->get_id(), 0));
    sim.run_until(200);
    assert(y->get_value() == 0);
    
    // A=1, B=0 -> Y=1
    sim.schedule_event(Event(300, a->get_id(), 1));
    sim.run_until(500);
    assert(y->get_value() == 1);
    
    delete a;
    delete b;
    delete y;
    delete or_gate;
    
    std::cout << "✓ OR gate test passed!\n";
}

void test_not_gate_basic() {
    std::cout << "\n=== Test: NOT Gate Basic Logic ===\n";
    
    Simulator sim;
    
    Signal* a = new Signal("A", 0);
    Signal* y = new Signal("Y", 2);
    
    sim.add_signal(a);
    sim.add_signal(y);
    
    NOTGate* not_gate = new NOTGate(50);
    not_gate->connect_input(a);
    not_gate->connect_output(y);
    sim.add_gate(not_gate);
    
    // A=0 -> Y=1
    sim.schedule_event(Event(0, a->get_id(), 0));
    sim.run_until(100);
    assert(y->get_value() == 1);
    
    // A=1 -> Y=0
    sim.schedule_event(Event(200, a->get_id(), 1));
    sim.run_until(300);
    assert(y->get_value() == 0);
    
    delete a;
    delete y;
    delete not_gate;
    
    std::cout << "✓ NOT gate test passed!\n";
}

void test_propagation_delay() {
    std::cout << "\n=== Test: Propagation Delay ===\n";
    
    Simulator sim;
    
    Signal* a = new Signal("A", 0);
    Signal* y = new Signal("Y", 2);
    
    sim.add_signal(a);
    sim.add_signal(y);
    
    NOTGate* not_gate = new NOTGate(100);  // 100ps delay
    not_gate->connect_input(a);
    not_gate->connect_output(y);
    sim.add_gate(not_gate);
    
    // Schedule A=1 at t=0
    sim.schedule_event(Event(0, a->get_id(), 1));
    
    // At t=50, Y should still be unknown (2)
    sim.run_until(50);
    assert(y->get_value() == 2);
    std::cout << "✓ At t=50ps: Y still unknown (delay not elapsed)\n";
    
    // At t=100, Y should be 0 (NOT of 1)
    sim.run_until(150);
    assert(y->get_value() == 0);
    std::cout << "✓ At t=100ps: Y = 0 (delay elapsed)\n";
    
    delete a;
    delete y;
    delete not_gate;
    
    std::cout << "✓ Propagation delay test passed!\n";
}

int main() {
    test_and_gate_basic();
    test_or_gate_basic();
    test_not_gate_basic();
    test_propagation_delay();
    
    std::cout << "\n=========================\n";
    std::cout << "✓ All Integration Tests Passed!\n";
    std::cout << "=========================\n";
    
    return 0;
}

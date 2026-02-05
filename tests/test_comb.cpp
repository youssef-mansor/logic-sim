#include "simulator.h"
#include "signal.h"
#include "gate.h"
#include "event.h"
#include <iostream>
#include <cassert>

void test_half_adder() {
    std::cout << "\n=== Test: Half Adder ===\n";
    
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
    std::cout << "\nTest 2: A=0, B=1 -> Sum=1, Carry=0\n";
    sim.schedule_event(Event(300, b->get_id(), 1));
    sim.run_until(500);
    assert(sum->get_value() == 1);
    assert(carry->get_value() == 0);
    std::cout << "✓ Passed\n";
    
    // Test case 3: 1 + 0 = 01
    std::cout << "\nTest 3: A=1, B=0 -> Sum=1, Carry=0\n";
    sim.schedule_event(Event(600, a->get_id(), 1));
    sim.schedule_event(Event(600, b->get_id(), 0));
    sim.run_until(800);
    assert(sum->get_value() == 1);
    assert(carry->get_value() == 0);
    std::cout << "✓ Passed\n";
    
    // Test case 4: 1 + 1 = 10
    std::cout << "\nTest 4: A=1, B=1 -> Sum=0, Carry=1\n";
    sim.schedule_event(Event(900, b->get_id(), 1));
    sim.run_until(1100);
    assert(sum->get_value() == 0);
    assert(carry->get_value() == 1);
    std::cout << "✓ Passed\n";
    
    // Output trace
    std::cout << "\n";
    sim.print_trace();
    sim.dump_waveform("half_adder.vcd");
    
    std::cout << "\n✓ Half adder test passed!\n";
}

void test_full_adder() {

    uint8_t delay = 0;

    std::cout << "\n=== Test: Full Adder (2 Half-Adders + OR) ===\n";
    
    Simulator sim;
    sim.enable_trace();
    
    // Input signals
    Signal* a = sim.create_signal("A", 0);
    Signal* b = sim.create_signal("B", 0);
    Signal* cin = sim.create_signal("Cin", 0);
    
    // Output signals
    Signal* sum = sim.create_signal("Sum", 2);
    Signal* cout = sim.create_signal("Cout", 2);
    
    // Internal signals
    Signal* sum1 = sim.create_signal("sum1", 2);    // First half-adder sum
    Signal* carry1 = sim.create_signal("carry1", 2); // First half-adder carry
    Signal* carry2 = sim.create_signal("carry2", 2); // Second half-adder carry
    
    // First half-adder: A + B
    XORGate* xor1 = sim.create_gate<XORGate>(delay);
    xor1->connect_input(a);
    xor1->connect_input(b);
    xor1->connect_output(sum1);
    
    ANDGate* and1 = sim.create_gate<ANDGate>(delay);
    and1->connect_input(a);
    and1->connect_input(b);
    and1->connect_output(carry1);
    
    // Second half-adder: sum1 + Cin
    XORGate* xor2 = sim.create_gate<XORGate>(delay);
    xor2->connect_input(sum1);
    xor2->connect_input(cin);
    xor2->connect_output(sum);
    
    ANDGate* and2 = sim.create_gate<ANDGate>(delay);
    and2->connect_input(sum1);
    and2->connect_input(cin);
    and2->connect_output(carry2);
    
    // Final OR for carry out
    ORGate* or_gate = sim.create_gate<ORGate>(delay);
    or_gate->connect_input(carry1);
    or_gate->connect_input(carry2);
    or_gate->connect_output(cout);
    
    // Test all 8 combinations
    struct TestCase {
        uint8_t a, b, cin;
        uint8_t expected_sum, expected_cout;
        std::string desc;
    };
    
    std::vector<TestCase> tests = {
        {0, 0, 0,  0, 0, "0+0+0=00"},
        {0, 0, 1,  1, 0, "0+0+1=01"},
        {0, 1, 0,  1, 0, "0+1+0=01"},
        {0, 1, 1,  0, 1, "0+1+1=10"},
        {1, 0, 0,  1, 0, "1+0+0=01"},
        {1, 0, 1,  0, 1, "1+0+1=10"},
        {1, 1, 0,  0, 1, "1+1+0=10"},
        {1, 1, 1,  1, 1, "1+1+1=11"}
    };
    
    uint64_t time = 0;
    for (size_t i = 0; i < tests.size(); i++) {
        const auto& tc = tests[i];
        
        std::cout << "\nTest " << (i+1) << ": " << tc.desc << "\n";
        
        // Schedule inputs
        sim.schedule_event(Event(time, a->get_id(), tc.a));
        sim.schedule_event(Event(time, b->get_id(), tc.b));
        sim.schedule_event(Event(time, cin->get_id(), tc.cin));
        
        // Run until signals propagate (3 levels of gates: 300ps)
        sim.run_until(time + 400);
        
        // Verify outputs
        
        std::cout << "  A=" << (int)tc.a << " B=" << (int)tc.b 
                  << " Cin=" << (int)tc.cin
                  << " -> Sum=" << (int)sum->get_value() 
                  << " Cout=" << (int)cout->get_value() << " ✓\n";

        assert(sum->get_value() == tc.expected_sum);
        assert(cout->get_value() == tc.expected_cout);
        

        
        time += 500; // Space out test cases
    }
    
    // Output trace
    std::cout << "\n";
    sim.print_trace();
    sim.dump_waveform("full_adder.vcd");
    
    std::cout << "\n✓ Full adder test passed (all 8 cases)!\n";
}


int main() {
    //test_half_adder();
    test_full_adder();
    
    std::cout << "\n=========================\n";
    std::cout << "✓ All Integration Tests Passed!\n";
    std::cout << "=========================\n";
    
    return 0;
}

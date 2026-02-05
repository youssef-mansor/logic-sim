#include "simulator.h"
#include "signal.h"
#include "sequential.h"
#include "event.h"
#include <iostream>
#include <cassert>

void test_dff_basic_capture() {
    std::cout << "\n=== Test: DFF Basic Capture ===\n";
    
    Simulator sim;
    sim.enable_trace();
    
    // Create signals
    Signal* clk = sim.create_signal("clk", 0);
    Signal* d = sim.create_signal("D", 0);
    Signal* q = sim.create_signal("Q", 2);  // Unknown initially
    
    // Create DFF with 50ps delay
    DFF* dff = new DFF(50);
    dff->connect_clock(clk);
    dff->connect_data(d);
    dff->connect_q(q);
    sim.add_component(dff);
    
    // Test 1: Set D=1, then rising edge on clock
    std::cout << "\nTest 1: D=1, then rising clock edge\n";
    sim.schedule_event(Event(0, d->get_id(), 1));
    sim.schedule_event(Event(100, clk->get_id(), 1));  // Rising edge
    sim.run_until(200);
    
    assert(q->get_value() == 1);
    std::cout << "✓ Q captured D=1 after rising edge\n";
    
    // Test 2: Clock falls, Q should remain stable
    std::cout << "\nTest 2: Falling edge, Q should hold value\n";
    sim.schedule_event(Event(300, clk->get_id(), 0));  // Falling edge
    sim.run_until(400);
    assert(q->get_value() == 1);
    std::cout << "✓ Q held value on falling edge\n";
    
    // Test 3: Change D while clock is low (no capture)
    std::cout << "\nTest 3: D changes while clock=0 (no capture)\n";
    sim.schedule_event(Event(500, d->get_id(), 0));
    sim.run_until(600);
    assert(q->get_value() == 1);  // Q should still be 1
    std::cout << "✓ Q did not change when D changed (clock low)\n";
    
    // Test 4: Rising edge captures new D value
    std::cout << "\nTest 4: Rising edge captures new D=0\n";
    sim.schedule_event(Event(700, clk->get_id(), 1));  // Rising edge
    sim.run_until(800);
    assert(q->get_value() == 0);
    std::cout << "✓ Q captured D=0 after rising edge\n";
    
    sim.print_trace();
    sim.dump_waveform("dff_basic.vcd");
    
    delete dff;
    std::cout << "\n✓ DFF basic capture test passed!\n";
}

void test_dff_multiple_captures() {
    std::cout << "\n=== Test: DFF Multiple Captures ===\n";
    
    Simulator sim;
    sim.enable_trace();
    
    Signal* clk = sim.create_signal("clk", 0);
    Signal* d = sim.create_signal("D", 0);
    Signal* q = sim.create_signal("Q", 2);
    
    DFF* dff = new DFF(50);
    dff->connect_clock(clk);
    dff->connect_data(d);
    dff->connect_q(q);
    sim.add_component(dff);
    
    // Sequence: 0 -> 1 -> 1 -> 0 -> 1
    uint64_t time = 0;
    uint8_t test_sequence[] = {0, 1, 1, 0, 1};
    
    for (int i = 0; i < 5; i++) {
        // Set D value
        sim.schedule_event(Event(time, d->get_id(), test_sequence[i]));
        
        // Rising clock edge
        sim.schedule_event(Event(time + 50, clk->get_id(), 1));
        sim.run_until(time + 150);
        
        // Check Q captured the value
        assert(q->get_value() == test_sequence[i]);
        std::cout << "  Cycle " << i << ": D=" << (int)test_sequence[i] 
                  << " -> Q=" << (int)q->get_value() << " ✓\n";
        
        // Falling edge
        sim.schedule_event(Event(time + 200, clk->get_id(), 0));
        sim.run_until(time + 250);
        
        time += 300;
    }
    
    sim.dump_waveform("dff_sequence.vcd");
    
    delete dff;
    std::cout << "\n✓ Multiple capture test passed!\n";
}

void test_dff_async_reset() {
    std::cout << "\n=== Test: DFF Asynchronous Reset ===\n";
    
    Simulator sim;
    sim.enable_trace();
    
    Signal* clk = sim.create_signal("clk", 0);
    Signal* d = sim.create_signal("D", 0);
    Signal* q = sim.create_signal("Q", 2);
    Signal* rst = sim.create_signal("rst", 0);
    
    DFF* dff = new DFF(50);
    dff->connect_clock(clk);
    dff->connect_data(d);
    dff->connect_q(q);
    dff->connect_reset(rst);
    sim.add_component(dff);
    
    // Test 1: Capture D=1
    std::cout << "\nTest 1: Capture D=1\n";
    sim.schedule_event(Event(0, d->get_id(), 1));
    sim.schedule_event(Event(100, clk->get_id(), 1));
    sim.run_until(200);
    assert(q->get_value() == 1);
    std::cout << "✓ Q=1 captured\n";
    
    // Test 2: Assert reset (should override and set Q=0)
    std::cout << "\nTest 2: Assert async reset\n";
    sim.schedule_event(Event(300, rst->get_id(), 1));
    sim.run_until(400);
    assert(q->get_value() == 0);
    std::cout << "✓ Q reset to 0 asynchronously\n";
    
    // Test 3: Clock edge while reset is high (should not capture)
    std::cout << "\nTest 3: Clock edge during reset\n";
    sim.schedule_event(Event(500, clk->get_id(), 0));
    sim.schedule_event(Event(600, clk->get_id(), 1));
    sim.run_until(700);
    assert(q->get_value() == 0);  // Still 0
    std::cout << "✓ Q stayed 0 during reset\n";
    
    // Test 4: Deassert reset, then capture works again
    std::cout << "\nTest 4: Deassert reset and capture\n";
    sim.schedule_event(Event(800, rst->get_id(), 0));
    sim.schedule_event(Event(900, clk->get_id(), 0));
    sim.schedule_event(Event(1000, clk->get_id(), 1));
    sim.run_until(1100);
    assert(q->get_value() == 1);  // Captured D=1
    std::cout << "✓ Normal operation resumed after reset\n";
    
    sim.dump_waveform("dff_reset.vcd");
    
    delete dff;
    std::cout << "\n✓ Async reset test passed!\n";
}

void test_dff_enable() {
    std::cout << "\n=== Test: DFF with Enable ===\n";
    
    Simulator sim;
    sim.enable_trace();
    
    Signal* clk = sim.create_signal("clk", 0);
    Signal* d = sim.create_signal("D", 0);
    Signal* q = sim.create_signal("Q", 2);
    Signal* en = sim.create_signal("enable", 1);  // Enabled by default
    
    DFF* dff = new DFF(50);
    dff->connect_clock(clk);
    dff->connect_data(d);
    dff->connect_q(q);
    dff->connect_enable(en);
    sim.add_component(dff);
    
    // Test 1: Capture when enabled
    std::cout << "\nTest 1: Capture with enable=1\n";
    sim.schedule_event(Event(0, d->get_id(), 1));
    sim.schedule_event(Event(100, clk->get_id(), 1));
    sim.run_until(200);
    assert(q->get_value() == 1);
    std::cout << "✓ Q=1 captured when enabled\n";
    
    // Test 2: Disable, change D, clock edge (should not capture)
    std::cout << "\nTest 2: Clock edge with enable=0\n";
    sim.schedule_event(Event(300, en->get_id(), 0));  // Disable
    sim.schedule_event(Event(400, d->get_id(), 0));
    sim.schedule_event(Event(500, clk->get_id(), 0));
    sim.schedule_event(Event(600, clk->get_id(), 1));
    sim.run_until(700);
    assert(q->get_value() == 1);  // Still 1, didn't capture
    std::cout << "✓ Q held value when disabled\n";
    
    // Test 3: Re-enable and capture
    std::cout << "\nTest 3: Re-enable and capture\n";
    sim.schedule_event(Event(800, en->get_id(), 1));
    sim.schedule_event(Event(900, clk->get_id(), 0));
    sim.schedule_event(Event(1000, clk->get_id(), 1));
    sim.run_until(1100);
    assert(q->get_value() == 0);  // Captured D=0
    std::cout << "✓ Q captured new value when re-enabled\n";
    
    sim.dump_waveform("dff_enable.vcd");
    
    delete dff;
    std::cout << "\n✓ Enable test passed!\n";
}

int main() {
    test_dff_basic_capture();
    test_dff_multiple_captures();
    test_dff_async_reset();
    test_dff_enable();
    
    std::cout << "\n=========================\n";
    std::cout << "✓ All DFF Tests Passed!\n";
    std::cout << "=========================\n";
    
    return 0;
}

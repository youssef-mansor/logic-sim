#include "signal.h"
#include <cassert>
#include <iostream>

void test_signal_creation() {
    Signal s1("clk", 0);
    assert(s1.get_name() == "clk");
    assert(s1.get_value() == 0);
    
    Signal s2("data", 2);  // Unknown state
    assert(s2.get_value() == 2);
    assert(s2.value_to_string() == "X");
    
    std::cout << "✓ Signal creation test passed\n";
}

void test_value_updates() {
    Signal sig("wire1", 0);
    
    sig.set_value(1);
    assert(sig.get_value() == 1);
    
    sig.set_value(2);  // Unknown
    assert(sig.get_value() == 2);
    
    std::cout << "✓ Value update test passed\n";
}

void test_invalid_value() {
    Signal sig("test", 0);
    try {
        sig.set_value(5);  // Invalid
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        std::cout << "✓ Invalid value rejection test passed\n";
    }
}

void test_observer_attachment() {
    Signal sig("input", 1);
    assert(sig.get_observers().empty());
    
    // We'll add actual gate attachment in Sprint 4
    // For now just verify the vector exists
    std::cout << "✓ Observer infrastructure test passed\n";
}

int main() {
    test_signal_creation();
    test_value_updates();
    test_invalid_value();
    test_observer_attachment();
    
    std::cout << "\n=== Sprint 2 Tests Passed ✓ ===\n";
    return 0;
}

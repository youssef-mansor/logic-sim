#include "event_queue.h"
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <cmath>
#include <cassert>

void test_large_scale_performance() {
    std::cout << "\n=== Large Scale Performance Test ===\n";
    
    std::vector<size_t> test_sizes = {100000, 250000, 500000, 750000, 1000000};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> time_dist(0, 1000000);
    std::uniform_int_distribution<int> signal_dist(0, 999);
    std::uniform_int_distribution<uint8_t> value_dist(0, 1);
    
    std::cout << "N\t\tTime(µs)\tAvg per insert(ns)\n";
    std::cout << "---------------------------------------------------\n";
    
    for (size_t n : test_sizes) {
        EventQueue eq;
        
        // Pre-generate events
        std::vector<Event> events;
        events.reserve(n);
        for (size_t i = 0; i < n; ++i) {
            events.emplace_back(time_dist(gen), signal_dist(gen), value_dist(gen));
        }
        
        // Measure insertion time
        auto start = std::chrono::high_resolution_clock::now();
        for (const auto& e : events) {
            eq.schedule(e);
        }
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        double avg_ns = (duration * 1000.0) / n;
        
        std::cout << n << "\t\t" << duration << "\t\t" << avg_ns << "\n";
        
        // Verify all events are retrievable
        assert(eq.size() == n);
    }
    
    std::cout << "\n✓ Large scale test passed\n";
    std::cout << "Note: O(log n) complexity means avg time should grow slowly\n";
    std::cout << "      (roughly 10-30% increase when N doubles)\n";
}

void test_heap_property() {
    std::cout << "\n=== Heap Property Verification ===\n";
    
    EventQueue eq;
    std::random_device rd;
    std::mt19937 gen(42);  // Fixed seed for reproducibility
    std::uniform_int_distribution<uint64_t> time_dist(0, 100000);

    // Insert 1000000 events with random times
    for (int i = 0; i < 1000000; ++i) {
        eq.schedule(Event(time_dist(gen), i, 0));
    }
    
    // Pop all and verify they come out in sorted order
    uint64_t prev_time = 0;
    int count = 0;
    while (!eq.empty()) {
        Event e = eq.pop_next();
        assert(e.time >= prev_time);  // Must be non-decreasing
        prev_time = e.time;
        count++;
    }

    assert(count == 1000000);
    std::cout << "✓ All 1000000 events popped in correct time order\n";
}

int main() {
    test_large_scale_performance();
    test_heap_property();
    
    std::cout << "\n=== All Sprint 1/4 Tests Passed ✓ ===\n";
    return 0;
}

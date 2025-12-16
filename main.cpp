#include <iostream>
#include <queue>
#include <string>

// -----------------------------
// Signal
// -----------------------------
struct Signal {
    std::string name;
    bool value;
};

// -----------------------------
// Event
// -----------------------------
struct Event {
    int time;
    Signal* signal;
    bool new_value;
};

// Compare events by time (earliest first)
struct EventCompare {
    bool operator()(const Event& a, const Event& b) const {
        return a.time > b.time;
    }
};

// -----------------------------
// NOT gate behavior (manual for Day 1)
// -----------------------------
void update_not_gate(const Signal& input, Signal& output) {
    output.value = !input.value;
}

// -----------------------------
// Main
// -----------------------------
int main() {
    // Initial signals
    Signal A{"A", false};  // A = 0
    Signal B{"B", true};   // B = NOT A = 1

    // Event queue
    std::priority_queue<Event, std::vector<Event>, EventCompare> event_queue;

    // Schedule an event: at time 10, A becomes 1
    event_queue.push(Event{10, &A, true});

    int current_time = 0;

    // Initial state
    std::cout << "[time " << current_time << "] "
              << "A = " << A.value << ", B = " << B.value << std::endl;

    // Simulation loop
    while (!event_queue.empty()) {
        Event e = event_queue.top();
        event_queue.pop();

        current_time = e.time;

        // Apply event
        e.signal->value = e.new_value;
        std::cout << "[time " << current_time << "] "
                  << e.signal->name << " changed to " << e.signal->value
                  << std::endl;

        // Manually update NOT gate
        update_not_gate(A, B);
        std::cout << "[time " << current_time << "] "
                  << "B updated to " << B.value << " (NOT A)"
                  << std::endl;
    }

    return 0;
}

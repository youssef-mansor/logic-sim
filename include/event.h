#ifndef EVENT_H
#define EVENT_H

#include <cstdint>
#include <string>

struct Event {
    uint64_t time; // Simulation time in picoseconds
    int signal_id; // Unique identifier for the signal
    uint8_t new_value; // New value of the signal (0, 1 or 'X' for unknown)

    // Constructor
    Event(uint64_t t, int id, uint8_t val);

    // For debugging
    std::string to_string() const;
};

#endif // EVENT_H
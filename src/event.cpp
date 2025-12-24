#include "event.h"

Event::Event(uint64_t t, int id, uint8_t val) : time(t), signal_id(id), new_value(val) {}

std::string Event::to_string() const {
    return "Event(time: " + std::to_string(time) + ", signal_id: " + std::to_string(signal_id) + ", new_value: " + std::to_string(new_value) + ")";
}
#include "event_queue.h"
#include <stdexcept>


void EventQueue::schedule(const Event& e) {
    pq.push(e);
}

Event EventQueue::pop_next() {
    if (pq.empty()) {
        throw std::runtime_error("EventQueue is empty");
    }
    Event next_event = pq.top();
    pq.pop();
    return next_event;
}

bool EventQueue::empty() const {
    return pq.empty();
}

size_t EventQueue::size() const {
    return pq.size();
}

uint64_t EventQueue::next_time() const {
    if (pq.empty()) {
        throw std::runtime_error("EventQueue is empty");
    }
    return pq.top().time;
}

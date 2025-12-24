#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "event.h"
#include <queue>
#include <vector>

struct EventComparator {
    bool operator()(const Event& a, const Event& b) const{
        return a.time > b.time; // Min-heap based on event time
    }
};

class EventQueue {
private:
    std::priority_queue<Event, std::vector<Event>, EventComparator> pq;
    
public:
    void schedule(const Event& e);
    Event pop_next();
    bool empty() const;
    size_t size() const;
    uint64_t next_time() const;  // Peek at next event time without popping
};

#endif // EVENT_QUEUE_H
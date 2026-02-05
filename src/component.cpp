#include "component.h"

uint64_t Component::get_delay() const {
    return propagation_delay;
}

std::string Component::get_id() const {
    return id;
}
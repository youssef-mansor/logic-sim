add_executable(sim_test
    src/event.cpp
    src/event_queue.cpp
    tests/test_event.cpp
)

target_include_directories(sim_test PRIVATE include)
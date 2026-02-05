add_executable(event_test
    src/event.cpp
    src/event_queue.cpp
    tests/test_event.cpp
)

target_include_directories(event_test PRIVATE include)

add_executable(signal_test
    src/signal.cpp
    tests/test_signal.cpp
)

target_include_directories(signal_test PRIVATE include)

add_executable(test_integration
    tests/test_integration.cpp
    src/event.cpp
    src/event_queue.cpp
    src/signal.cpp
    src/gate.cpp
    src/component.cpp
    src/simulator.cpp
)

target_include_directories(test_integration PRIVATE include)

add_executable(test_trace_waveform
    tests/test_trace_waveform.cpp
    src/event.cpp
    src/event_queue.cpp
    src/signal.cpp
    src/gate.cpp
    src/component.cpp
    src/simulator.cpp
)

target_include_directories(test_trace_waveform PRIVATE include)

add_executable(test_comb
    tests/test_comb.cpp
    src/event.cpp
    src/event_queue.cpp
    src/signal.cpp
    src/gate.cpp
    src/component.cpp
    src/simulator.cpp
)

target_include_directories(test_comb PRIVATE include)

add_executable(test_dff
    tests/test_dff.cpp
    src/event.cpp
    src/event_queue.cpp
    src/signal.cpp
    src/component.cpp
    src/sequential.cpp
    src/simulator.cpp
)

target_include_directories(test_dff PRIVATE include)
cmake_minimum_required(VERSION 3.17)
project(simulator)

set(CMAKE_CXX_STANDARD 20)

set(SIM_SOURCES
    src/cpu.cpp
    src/simulator.cpp
    src/stages/Fetch.cpp
    src/stages/Decode.cpp
    src/stages/Execute.cpp
    src/stages/Memory.cpp
    src/stages/WriteBack.cpp
    src/units/ControlUnit.cpp
    src/units/HazardUnit.cpp
    src/riscv/instruction.cpp
)

include_directories(src)

add_executable(cpu
    ${SIM_SOURCES}
)

set(SIM_TESTS
    tests/RV32I_tests.cpp
)

add_executable(tests
    ${SIM_TESTS}
)

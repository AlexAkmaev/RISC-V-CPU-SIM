cmake_minimum_required(VERSION 3.17)
project(simulator)

set(CMAKE_CXX_STANDARD 20)

set(SIM_SOURCES
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
    src/cpu.cpp
    ${SIM_SOURCES}
)

set(SIM_TESTS
    ${SIM_SOURCES}
    tests/RV32I_tests.cpp
)

add_executable(tests
    ${SIM_TESTS}
)

cmake_minimum_required(VERSION 3.17)
project(simulator)

set(CMAKE_CXX_STANDARD 17)

set(SIM_SOURCES
        src/cpu.cpp
        src/simulator.cpp
        src/stages/Fetch.cpp
        src/stages/Decode.cpp
        src/units/ControlUnit.cpp
        src/riscv/instruction.cpp)

include_directories(src)

add_executable(cpu
    ${SIM_SOURCES}
)

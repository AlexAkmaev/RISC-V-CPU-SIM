cmake_minimum_required(VERSION 3.17)
project(simulator)

set(CMAKE_CXX_STANDARD 17)

set(SIM_SOURCES
        src/cpu.cpp
        src/simulator.cpp
        src/stages/Fetch.cpp
        src/riscv/instruction.cpp)

add_executable(cpu
    ${SIM_SOURCES}
)

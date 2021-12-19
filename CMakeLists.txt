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

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(BaseInstructionsTests
    ${SIM_SOURCES}
    tests/BaseInstructionsTests.cpp
)

set(HazardUnitTests
    ${SIM_SOURCES}
    tests/HazardUnitTests.cpp
)


# Link runTests with what we want to test and the GTest and pthread library
add_executable(BaseInstructionsTests ${BaseInstructionsTests})
target_link_libraries(BaseInstructionsTests ${GTEST_LIBRARIES} pthread)

add_executable(HazardUnitTests ${HazardUnitTests})
target_link_libraries(HazardUnitTests ${GTEST_LIBRARIES} pthread)

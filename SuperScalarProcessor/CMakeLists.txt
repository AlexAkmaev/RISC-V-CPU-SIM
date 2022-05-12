cmake_minimum_required(VERSION 3.17)
project(riscv-simulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror" CACHE STRING "Default CXX options" FORCE)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(riscv)
add_subdirectory(stages)
add_subdirectory(units)

set(CPU_SOURCES cpu.cpp)
add_executable(cpu ${CPU_SOURCES})
target_link_libraries(cpu riscv stages units)

include(CTest)
enable_testing()

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

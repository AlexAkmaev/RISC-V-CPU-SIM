cmake_minimum_required(VERSION 3.17)

set(UNITS_SOURCES
    ControlUnit.cpp
    HazardUnit.cpp
)

add_library(units ${UNITS_SOURCES})
target_include_directories(units PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(units riscv stages)

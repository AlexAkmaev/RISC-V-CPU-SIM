cmake_minimum_required(VERSION 3.17)

set(RISCV_SOURCES
    instruction.cpp
    simulator.cpp
)

add_library(riscv ${RISCV_SOURCES})
target_include_directories(riscv
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/common
)
target_link_libraries(riscv units stages)

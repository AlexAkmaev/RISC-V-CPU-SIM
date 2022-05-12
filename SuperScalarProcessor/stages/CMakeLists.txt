cmake_minimum_required(VERSION 3.17)

set(STAGE_SOURCES
    Fetch.cpp
    Decode.cpp
    Execute.cpp
    Memory.cpp
    WriteBack.cpp
)

add_library(stages ${STAGE_SOURCES})
target_include_directories(stages PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(stages riscv units)

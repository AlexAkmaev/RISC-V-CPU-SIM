cmake_minimum_required(VERSION 3.17)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

set(BaseInstructionsTests BaseInstructionsTests.cpp)
set(BlocksTests BlocksTests.cpp)
set(HazardUnitTests HazardUnitTests.cpp)
set(SuperScalarTests SuperScalarTests.cpp)

add_executable(base_instructions_tests ${BaseInstructionsTests})
target_link_libraries(base_instructions_tests PRIVATE GTest::GTest riscv stages units)
add_test(base_instructions_tests_gtests base_instructions_tests)

add_executable(blocks_tests ${BlocksTests})
target_link_libraries(blocks_tests PRIVATE GTest::GTest riscv stages units)
add_test(blocks_tests_gtests blocks_tests)

add_executable(hazard_unit_tests ${HazardUnitTests})
target_link_libraries(hazard_unit_tests PRIVATE GTest::GTest riscv stages units)
add_test(hazard_unit_tests_gtests hazard_unit_tests)

add_executable(super_scalar_tests ${SuperScalarTests})
target_link_libraries(super_scalar_tests PRIVATE GTest::GTest riscv stages units)
add_test(super_scalar_tests_gtests super_scalar_tests)

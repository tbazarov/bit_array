cmake_minimum_required(VERSION 3.10)
project(bit_array_project)

set(CMAKE_CXX_STANDARD 17)

add_library(bit_array bit_arr.cpp bit_arr.hpp)

enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tests bit_arr_test.cpp)

target_link_libraries(tests
    bit_array
    GTest::GTest
    GTest::Main
)

add_test(NAME bit_array_tests COMMAND tests)
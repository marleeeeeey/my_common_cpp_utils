cmake_minimum_required(VERSION 3.14)

project(my_cpp_utils_tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(my_cpp_utils_tests
    config_tests.cpp
    math_tests.cpp
    json_formatter_tests.cpp
)

target_include_directories(my_cpp_utils_tests
    PRIVATE
    ${gtest_SOURCE_DIR}/include
    ${gmock_SOURCE_DIR}/include
)

target_link_libraries(my_cpp_utils_tests
    PRIVATE
    gtest
    gtest_main
    gmock
    my_cpp_utils
    glm
)

include(GoogleTest)
gtest_discover_tests(my_cpp_utils_tests)

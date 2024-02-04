cmake_minimum_required(VERSION 3.15)

project(my_common_cpp_utils)

add_library(my_common_cpp_utils
    STATIC
    src/Config.cpp
    src/JsonLoader.cpp
)

target_include_directories(my_common_cpp_utils
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# IMPORTANT: Setup dependencies
# Here it is assumed that libraries mentioned below are already included in the parent CMakeLists.txt
# If this is not the case, you will need to add find_package or similar commands here
target_link_libraries(my_common_cpp_utils
    PUBLIC
    nlohmann_json::nlohmann_json
    magic_enum::magic_enum
    spdlog::spdlog
    glm
)

add_subdirectory(tests)
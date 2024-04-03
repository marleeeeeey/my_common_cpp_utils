cmake_minimum_required(VERSION 3.15)
project(my_cpp_utils)

# ###################################################################
# #################### Setup compiler options #######################
# ###################################################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Generate compile_commands.json.

# ###################################################################
# ############### Searching some packages in system #################
# ###################################################################
find_package(nlohmann_json CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# ###################################################################
# #################### Build library itself #########################
# ###################################################################
file(GLOB_RECURSE MY_CPP_UTILS_SOURCES "./src/*.cpp")
add_library(my_cpp_utils STATIC ${MY_CPP_UTILS_SOURCES})

target_include_directories(my_cpp_utils
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(my_cpp_utils
    PUBLIC
    nlohmann_json::nlohmann_json
    magic_enum::magic_enum
    spdlog::spdlog
    glm::glm-header-only
)

if(TARGET GTest::gtest)
    add_subdirectory(tests)
endif()

project(speller)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

add_library(speller_utility_library INTERFACE "utility.hpp")
target_include_directories(speller_utility_library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(speller "main.cpp")
target_link_libraries(speller speller_utility_library)

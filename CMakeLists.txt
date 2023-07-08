project(speller)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

add_library(speller_utility_library INTERFACE "include/speller/utility.hpp")
target_include_directories(speller_utility_library INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(speller_regex "src/regex_main.cpp")
target_link_libraries(speller_regex speller_utility_library)

add_executable(speller_search "src/search_main.cpp")
target_link_libraries(speller_search speller_utility_library)

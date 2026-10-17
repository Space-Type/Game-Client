##
## EPITECH PROJECT, 2026
## Project - R-Type
## File description:
## Unit test build
## file
##

cmake_minimum_required(VERSION 3.15)

# Provided by vcpkg ("tests" feature of vcpkg.json)
find_package(GTest CONFIG REQUIRED)

# Only headless tests: nothing here opens a window or an audio device
set(TEST_SOURCES
    render/ActionMap.cpp
    render/Concepts.cpp
    render/KeyMap.cpp
    render/KeyNames.cpp
    render/Letterbox.cpp
    render/Types.cpp)

add_executable(rtype_tests ${TEST_SOURCES})

# raylib for its key codes (render/KeyMap.cpp)
target_link_libraries(rtype_tests PRIVATE rtype_render
                                  PRIVATE raylib
                                  PRIVATE GTest::gtest_main)

set_target_properties(rtype_tests PROPERTIES MAP_IMPORTED_CONFIG_DEBUGTESTS Debug)

include(GoogleTest)
gtest_discover_tests(rtype_tests)

##
## EPITECH PROJECT, 2026
## Project - R-Type
## File description:
## Backend unit test build
## file
##

cmake_minimum_required(VERSION 3.15)

include(FetchContent)

# Same declaration as Wrecks: the first one wins, no second download
FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)

FetchContent_MakeAvailable(googletest)

# Only headless tests: nothing here opens a window or an audio device
set(BACKEND_TEST_SOURCES
    ActionMap.cpp
    Concepts.cpp
    KeyMap.cpp
    KeyNames.cpp
    Letterbox.cpp
    Types.cpp)

add_executable(backend_tests ${BACKEND_TEST_SOURCES})

# backend/src for private headers (Input/KeyMap.hpp); raylib for its key codes
target_include_directories(backend_tests PRIVATE ../src)

target_link_libraries(backend_tests PRIVATE rtype_backend
                                    PRIVATE raylib
                                    PRIVATE GTest::gtest_main)

set_target_properties(backend_tests PROPERTIES MAP_IMPORTED_CONFIG_DEBUGTESTS Debug)

include(GoogleTest)
gtest_discover_tests(backend_tests)

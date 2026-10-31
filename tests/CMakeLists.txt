##
## EPITECH PROJECT, 2026
## Project - R-Type
## File description:
## Unit test build
## file
##

cmake_minimum_required(VERSION 3.15)

include(FetchContent)

FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)

# MSVC: use the same (shared) runtime as the tested code
if (WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

# Only headless tests: nothing here opens a window or an audio device
set(TEST_SOURCES
    game/Client.cpp
    render/ActionMap.cpp
    render/Concepts.cpp
    render/KeyMap.cpp
    render/KeyNames.cpp
    render/Letterbox.cpp
    render/Types.cpp
    scenes/Manager.cpp
    ui/Form.cpp
    ui/TextField.cpp
    ui/Theme.cpp)

add_executable(rtype_tests ${TEST_SOURCES})

# raylib for its key codes (render/KeyMap.cpp)
target_link_libraries(rtype_tests PRIVATE rtype_client_core
                                  PRIVATE raylib
                                  PRIVATE GTest::gtest_main)

set_target_properties(rtype_tests PROPERTIES MAP_IMPORTED_CONFIG_DEBUGTESTS Debug)

include(GoogleTest)
gtest_discover_tests(rtype_tests)

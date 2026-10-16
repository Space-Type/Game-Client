##
## EPITECH PROJECT, 2026
## Project - R-Type
## File description:
## Main project build
## file
##

cmake_minimum_required(VERSION 3.15)

project(RType-Client VERSION 1.0
                     DESCRIPTION "R-Type Game Client - R-Type project"
                     HOMEPAGE_URL "https://github.com/Space-Type/Game-Client"
                     LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo;DebugTests"
                              CACHE STRING "Supported build types" FORCE)

set(CMAKE_CXX_FLAGS_DEBUGTESTS "${CMAKE_CXX_FLAGS_DEBUG}"
                               CACHE STRING "Flags used by the C++ compiler during DebugTests builds.")

# Lets `ctest --test-dir build` find the tests of every subproject
if (CMAKE_BUILD_TYPE STREQUAL "DebugTests")
    enable_testing()
endif()

add_subdirectory(lib/raylib)
add_subdirectory(lib/wrecks)
add_subdirectory(backend)

set(SOURCES
    src/main.cpp)


add_executable(r-type_client ${SOURCES})

target_include_directories(r-type_client PUBLIC include
                                         PUBLIC lib/wrecks/include)

target_link_libraries(r-type_client wrecks)

# Wrecks objects are built with --coverage in DebugTests: link libgcov
if (CMAKE_BUILD_TYPE STREQUAL "DebugTests" AND NOT WIN32)
    target_link_options(r-type_client PRIVATE --coverage)
endif()

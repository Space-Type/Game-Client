##
## EPITECH PROJECT, 2026
## Project - R-Type
## File description:
## Main project build
## file
##

cmake_minimum_required(VERSION 3.15)

# Third-party dependencies come from vcpkg (submodule lib/vcpkg, manifest
# vcpkg.json), bootstrapped automatically on the first configure.
# OpenGL and X11 stay system libraries, as the subject allows.
if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/lib/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "vcpkg toolchain file")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "DebugTests")
    list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()

project(RType-Client VERSION 1.0
                     DESCRIPTION "R-Type Game Client - R-Type project"
                     HOMEPAGE_URL "https://github.com/Space-Type/Game-Client"
                     LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo;DebugTests"
                              CACHE STRING "Supported build types" FORCE)

set(CMAKE_CXX_FLAGS_DEBUGTESTS "${CMAKE_CXX_FLAGS_DEBUG}"
                               CACHE STRING "Flags used by the C++ compiler during DebugTests builds.")

# Lets `ctest --test-dir build` find the tests of subdirectories
if (CMAKE_BUILD_TYPE STREQUAL "DebugTests")
    enable_testing()
endif()

add_subdirectory(lib/raylib)

# Wrecks builds its own tests (with coverage) in DebugTests:
# they belong to the Wrecks repository, not to this build
set(RTYPE_BUILD_TYPE "${CMAKE_BUILD_TYPE}")
if (CMAKE_BUILD_TYPE STREQUAL "DebugTests")
    set(CMAKE_BUILD_TYPE Debug)
endif()
add_subdirectory(lib/wrecks)
set(CMAKE_BUILD_TYPE "${RTYPE_BUILD_TYPE}")

add_subdirectory(backend)

set(SOURCES
    src/main.cpp)


add_executable(r-type_client ${SOURCES})

target_include_directories(r-type_client PUBLIC include
                                         PUBLIC lib/wrecks/include)

target_link_libraries(r-type_client wrecks)

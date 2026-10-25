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

# Wrecks and Protecs build their own tests (with coverage) in DebugTests:
# they belong to their repositories, not to this build
set(RTYPE_BUILD_TYPE "${CMAKE_BUILD_TYPE}")
if (CMAKE_BUILD_TYPE STREQUAL "DebugTests")
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_subdirectory(lib/wrecks)
add_subdirectory(lib/protecs)
set(CMAKE_BUILD_TYPE "${RTYPE_BUILD_TYPE}")

set(SOURCES
    src/components/Controllable.hpp
    src/components/Dead.hpp
    src/components/HasRandomMovement.hpp
    src/components/Health.hpp
    src/components/Hitbox.hpp
    src/components/Manager.hpp
    src/components/Projectile.hpp
    src/components/Shooter.hpp
    src/components/Sprite.hpp
    src/components/Star.hpp
    src/errors/Error/Error.cpp
    src/errors/Error/Error.hpp
    src/errors/GraphicsError/GraphicsError.cpp
    src/errors/GraphicsError/GraphicsError.hpp
    src/game/Controls/Controls.cpp
    src/game/Controls/Controls.hpp
    src/game/Game/Game.cpp
    src/game/Game/Game.hpp
    src/game/Engine.hpp
    src/render/Assets/Assets.cpp
    src/render/Assets/Assets.hpp
    src/render/Audio/Audio.cpp
    src/render/Audio/Audio.hpp
    src/render/Input/Input.cpp
    src/render/Input/Input.hpp
    src/render/Input/KeyMap.hpp
    src/render/Input/KeyNames.cpp
    src/render/Letterbox/Letterbox.cpp
    src/render/Letterbox/Letterbox.hpp
    src/render/Renderer/Renderer.cpp
    src/render/Renderer/Renderer.hpp
    src/render/Resources/Resources.cpp
    src/render/Resources/Resources.hpp
    src/render/Resources/ResourcesImpl.hpp
    src/render/Window/Window.cpp
    src/render/Window/Window.hpp
    src/render/ActionMap.hpp
    src/render/Concepts.hpp
    src/render/Convert.hpp
    src/render/Error.hpp
    src/render/InputTypes.hpp
    src/render/Log.hpp
    src/render/Types.hpp
    src/systems/Render/Render.cpp
    src/systems/Render/Render.hpp
    src/systems/RandomMovement/RandomMovement.cpp
    src/systems/RandomMovement/RandomMovement.hpp
    src/systems/Controllable/Controllable.cpp
    src/systems/Controllable/Controllable.hpp
    src/systems/Shoot/Shoot.cpp
    src/systems/Shoot/Shoot.hpp
    src/systems/Projectile/Projectile.cpp
    src/systems/Projectile/Projectile.hpp
    src/systems/Collision/Collision.cpp
    src/systems/Collision/Collision.hpp
    src/systems/Dead/Dead.cpp
    src/systems/Dead/Dead.hpp
    src/systems/Starfield/Starfield.cpp
    src/systems/Starfield/Starfield.hpp
    src/systems/StarRender/StarRender.cpp
    src/systems/StarRender/StarRender.hpp
    src/systems/Tuple.hpp
    src/main.cpp)

add_executable(r-type_client ${SOURCES})

# windows.h (included by the Protecs networking headers) clashes with raylib
# (CloseWindow, ShowCursor, Rectangle...) and defines min / max macros
if (WIN32)
    target_compile_definitions(r-type_client PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX NOGDI NOUSER)
endif()

target_include_directories(r-type_client PUBLIC include
                                         PUBLIC lib/wrecks/include
                                         PUBLIC lib/protecs/include
                                         PRIVATE lib/raylib/src)

target_link_libraries(r-type_client PUBLIC wrecks
                                    PUBLIC protecs
                                    PUBLIC raylib)

if (CMAKE_BUILD_TYPE STREQUAL "DebugTests")
    add_subdirectory(tests)
endif()

# Manual test program (needs a display): cmake --build build --target render_manual_test
add_subdirectory(tests/render/manual)

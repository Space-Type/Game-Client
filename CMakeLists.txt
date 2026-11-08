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

if (NOT TARGET wrecks)
    add_subdirectory(lib/wrecks)
endif()
if (NOT TARGET protecs)
    add_subdirectory(lib/protecs)
endif()

set(CMAKE_BUILD_TYPE "${RTYPE_BUILD_TYPE}")

set(SOURCES
    src/components/Charge.hpp
    src/components/Controllable.hpp
    src/components/EntityKind.hpp
    src/components/Freshness.hpp
    src/components/Lifetime.hpp
    src/components/Manager.hpp
    src/components/PlayerState.hpp
    src/components/Sprite.hpp
    src/errors/Error/Error.cpp
    src/errors/Error/Error.hpp
    src/errors/GraphicsError/GraphicsError.cpp
    src/errors/GraphicsError/GraphicsError.hpp
    src/errors/NetworkError/NetworkError.cpp
    src/errors/NetworkError/NetworkError.hpp
    src/game/ConnectionInfo/ConnectionInfo.cpp
    src/game/ConnectionInfo/ConnectionInfo.hpp
    src/game/Controls/Controls.cpp
    src/game/Controls/Controls.hpp
    src/game/Client/Client.cpp
    src/game/Client/Client.hpp
    src/game/DisplayOptions/DisplayOptions.hpp
    src/game/Game/Game.cpp
    src/game/Game/Game.hpp
    src/game/Preferences/Preferences.cpp
    src/game/Preferences/Preferences.hpp
    src/game/Session/Session.cpp
    src/game/SpriteSheets/SpriteSheets.cpp
    src/game/SpriteSheets/SpriteSheets.hpp
    src/game/Starfield/Starfield.cpp
    src/game/Starfield/Starfield.hpp
    src/game/Session/Session.hpp
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
    src/scenes/Connect/Connect.cpp
    src/scenes/Connect/Connect.hpp
    src/scenes/Connecting/Connecting.cpp
    src/scenes/Connecting/Connecting.hpp
    src/scenes/GameOver/GameOver.cpp
    src/scenes/GameOver/GameOver.hpp
    src/scenes/InGame/InGame.cpp
    src/scenes/InGame/InGame.hpp
    src/scenes/Manager/Manager.hpp
    src/scenes/Paused/Paused.cpp
    src/scenes/Paused/Paused.hpp
    src/scenes/Context.hpp
    src/scenes/SceneId.hpp
    src/scenes/Settings/Settings.cpp
    src/scenes/Settings/Settings.hpp
    src/scenes/Text.hpp
    src/systems/PlayerInput/PlayerInput.cpp
    src/systems/PlayerInput/PlayerInput.hpp
    src/systems/ChargeTimer/ChargeTimer.cpp
    src/systems/ChargeTimer/ChargeTimer.hpp
    src/systems/Expire/Expire.cpp
    src/systems/Expire/Expire.hpp
    src/systems/RenderCharge/RenderCharge.cpp
    src/systems/RenderCharge/RenderCharge.hpp
    src/systems/RenderName/RenderName.cpp
    src/systems/RenderName/RenderName.hpp
    src/systems/RenderOutline/RenderOutline.cpp
    src/systems/RenderOutline/RenderOutline.hpp
    src/systems/RenderSprite/RenderSprite.cpp
    src/systems/RenderStage/RenderStage.cpp
    src/systems/RenderStage/RenderStage.hpp
    src/systems/RenderSprite/RenderSprite.hpp
    src/systems/Tuple.hpp
    src/ui/Button/Button.cpp
    src/ui/Button/Button.hpp
    src/ui/Checkbox/Checkbox.cpp
    src/ui/Checkbox/Checkbox.hpp
    src/ui/Form/Form.cpp
    src/ui/Form/Form.hpp
    src/ui/FrameInput/FrameInput.cpp
    src/ui/FrameInput/FrameInput.hpp
    src/ui/Hud/Hud.cpp
    src/ui/Hud/Hud.hpp
    src/ui/MenuControls/MenuControls.cpp
    src/ui/MenuControls/MenuControls.hpp
    src/ui/Slider/Slider.cpp
    src/ui/Slider/Slider.hpp
    src/ui/TextField/TextField.cpp
    src/ui/TextField/TextField.hpp
    src/ui/Theme.hpp
    src/utils/EntityMap/EntityMap.cpp
    src/utils/EntityMap/EntityMap.hpp
    src/utils/ThreadQueue.hpp)

# Everything but the entry point, shared by the client and the unit tests
add_library(rtype_client_core STATIC ${SOURCES})

# windows.h (included by the Protecs networking headers) clashes with raylib
# (CloseWindow, ShowCursor, Rectangle...) and defines min / max macros
if (WIN32)
    target_compile_definitions(rtype_client_core PUBLIC WIN32_LEAN_AND_MEAN NOMINMAX NOGDI NOUSER)
endif()

target_include_directories(rtype_client_core PUBLIC  include
                                             PUBLIC  lib/wrecks/include
                                             PUBLIC  lib/protecs/include
                                             PRIVATE lib/raylib/src)

target_link_libraries(rtype_client_core PUBLIC wrecks
                                        PUBLIC protecs
                                        PUBLIC raylib)

add_executable(r-type_client src/main.cpp)

target_link_libraries(r-type_client PRIVATE rtype_client_core)

if (CMAKE_BUILD_TYPE STREQUAL "DebugTests")
    add_subdirectory(tests)
endif()

# Manual test program (needs a display): cmake --build build --target render_manual_test
add_subdirectory(tests/render/manual)

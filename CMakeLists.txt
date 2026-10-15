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

add_subdirectory(lib/wrecks)

set(SOURCES
    src/components/Manager.hpp
    src/components/Physics.hpp
    src/components/Transform.hpp
    src/game/Game/Game.cpp
    src/game/Game/Game.hpp
    src/game/Engine.hpp
    src/systems/Physics/Physics.cpp
    src/systems/Physics/Physics.hpp
    src/systems/Systems.hpp
    src/main.cpp)


add_executable(r-type_client ${SOURCES})

target_include_directories(r-type_client PUBLIC include
                                         PUBLIC lib/wrecks/include)

target_link_libraries(r-type_client wrecks)

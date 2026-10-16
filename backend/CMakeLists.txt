##
## EPITECH PROJECT, 2026
## Project - R-Type
## File description:
## Backend library build
## file
##

cmake_minimum_required(VERSION 3.15)

set(BACKEND_SOURCES
    include/backend.hpp
    include/backend/ActionMap.hpp
    include/backend/Audio.hpp
    include/backend/Concepts.hpp
    include/backend/Error.hpp
    include/backend/Input.hpp
    include/backend/InputTypes.hpp
    include/backend/Letterbox.hpp
    include/backend/Renderer.hpp
    include/backend/Resources.hpp
    include/backend/Types.hpp
    include/backend/Window.hpp

    # Common
    src/Log.hpp

    # Graphics
    src/Letterbox/Letterbox.cpp
    src/Convert.hpp
    src/Window/Window.cpp


    # Input & audio


    )

add_library(rtype_backend STATIC ${BACKEND_SOURCES})

target_include_directories(rtype_backend PUBLIC include
                                         PRIVATE src)

# raylib stays private: game code cannot include it
target_link_libraries(rtype_backend PUBLIC wrecks
                                    PRIVATE raylib)

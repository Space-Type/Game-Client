##
## EPITECH PROJECT, 2026
## Project - R-Type
## File description:
## Render manual test
## build file
##

cmake_minimum_required(VERSION 3.15)

# Manual test program, not built by default:
# cmake --build build --target render_manual_test
add_executable(render_manual_test EXCLUDE_FROM_ALL main.cpp)

target_link_libraries(render_manual_test PRIVATE rtype_client_core)

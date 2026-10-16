##
## EPITECH PROJECT, 2026
## Project - R-Type
## File description:
## Backend manual test
## build file
##

cmake_minimum_required(VERSION 3.15)

# Manual test program, not built by default:
# cmake --build build --target backend_manual_test
add_executable(backend_manual_test EXCLUDE_FROM_ALL main.cpp)

target_link_libraries(backend_manual_test PRIVATE rtype_backend)

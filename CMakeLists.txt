cmake_minimum_required(VERSION 3.20)
project(r-type LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2 COMPONENTS graphics window system REQUIRED)

add_executable(r-type_client
    client/src/main.cpp
    client/src/starfield.cpp
    client/src/spaceship.cpp
)

target_include_directories(r-type_client PRIVATE
    ${CMAKE_SOURCE_DIR}/client/include
)

target_link_libraries(r-type_client PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)
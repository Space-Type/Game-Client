/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Program Entry
*/

#include <rtype.hpp>
#include <wrecks.hpp>


int main(int argc, char *argv[])
{
    RType::Game::Engine engine;

    auto spaceship = engine.createEntity();
    engine.addComponent(spaceship, RType::Components::Transform{ .x = 0.0, .y = 0.0, .rotation = 0.0 });
    engine.addComponent(spaceship, RType::Components::Physics{ .velocityX = 10.0, .velocityY = 0.0 });

    while (true) {
        engine.update();
    }
}

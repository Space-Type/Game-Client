/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Physics System
*/

#include "Physics.hpp"
#include <rtype/game.hpp>

static constexpr float FRICTION_FACTOR_X = 0.9f;
static constexpr float FRICTION_FACTOR_Y = FRICTION_FACTOR_X;

RType::Systems::Physics::Physics() noexcept
{

}

#include <iostream>  // TODO: Remove
void RType::Systems::Physics::operator()(Wrecks::ECS::Entity entity)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    auto& physicsState = engine.get<Components::Physics>()[entity];
    auto& transforms = engine.get<Components::Transform>()[entity];

    transforms.x += physicsState.velocityX;
    transforms.y += physicsState.velocityY;

    physicsState.velocityX *= FRICTION_FACTOR_X;
    physicsState.velocityY *= FRICTION_FACTOR_Y;

    std::cout << "{ "
              << ".x = " << transforms.x << ", "
              << ".y = " << transforms.y << ", "
              << ".rotation = " << transforms.rotation
              << " }" << std::endl;
}

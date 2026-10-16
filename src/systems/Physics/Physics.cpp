/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Physics System
*/

#include "Physics.hpp"
#include <rtype/game.hpp>
#include <raylib.h>

static constexpr float FRICTION_FACTOR_X = 0.89f;
static constexpr float FRICTION_FACTOR_Y = FRICTION_FACTOR_X;

RType::Systems::Physics::Physics() noexcept
{

}

void RType::Systems::Physics::operator()(Wrecks::ECS::Entity entity)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    auto& physicsState = engine.get<Components::Physics>()[entity];
    auto& transforms = engine.get<Components::Transform>()[entity];

    physicsState.velocityX += 3.f * (IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT));
    physicsState.velocityY += 3.f * (IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP));

    physicsState.velocityX = std::clamp(physicsState.velocityX, -2.f, 2.f);
    physicsState.velocityY = std::clamp(physicsState.velocityY, -2.f, 2.f);

    transforms.x += physicsState.velocityX;
    transforms.y += physicsState.velocityY;

    physicsState.velocityX *= FRICTION_FACTOR_X;
    physicsState.velocityY *= FRICTION_FACTOR_Y;
}

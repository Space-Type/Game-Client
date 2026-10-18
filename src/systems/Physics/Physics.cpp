/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Physics System
*/

#include "Physics.hpp"
#include <rtype/game.hpp>
#include <algorithm>
#include <cmath>

RType::Systems::Physics::Physics() noexcept
{

}

void RType::Systems::Physics::operator()(Wrecks::ECS::Entity entity)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    auto& physicsState = engine.get<Components::Physics>()[entity];
    auto& transforms = engine.get<Components::Transform>()[entity];

    const float dt = *m_DeltaTime;

    const float maxSpeed = physicsState.maxSpeed;

    physicsState.velocityX = std::clamp(physicsState.velocityX, -maxSpeed, maxSpeed);
    physicsState.velocityY = std::clamp(physicsState.velocityY, -maxSpeed, maxSpeed);

    transforms.x += physicsState.velocityX * dt;
    transforms.y += physicsState.velocityY * dt;

    if (transforms.x > m_Window->virtualSize().x)
        transforms.x = m_Window->virtualSize().x;
    if (transforms.x < 0)
        transforms.x = 0;
    if (transforms.y > m_Window->virtualSize().y)
        transforms.y = m_Window->virtualSize().y;
    if (transforms.y < 0)
        transforms.y = 0;

    // Exponential slowdown: gives the same result whatever the framerate
    const float friction = std::exp(-physicsState.damping * dt);
    physicsState.velocityX *= friction;
    physicsState.velocityY *= friction;
}

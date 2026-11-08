/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** ScrollStars system
*/

#include "ScrollStars.hpp"
#include <rtype/game.hpp>
#include <chrono>


RType::Systems::ScrollStars::ScrollStars() noexcept
    : m_Engine(nullptr)
    , m_Area()
    , m_Random(1)
{

}

void RType::Systems::ScrollStars::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& star = engine.get<Components::Star>()[entity];

    if (!Components::scroll(star, std::chrono::duration<float>(dt).count(), m_Area.x))
        return;

    std::uniform_real_distribution<float> height(0.0F, m_Area.y);

    star.position.y = height(m_Random);
}

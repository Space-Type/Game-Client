/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Starfield System
*/

#include "Starfield.hpp"
#include <rtype/game.hpp>
#include <random>


RType::Systems::Starfield::Starfield() noexcept
    : m_Engine(nullptr)
    , m_Window(nullptr)
    , m_DeltaTime(nullptr)
{

}

void RType::Systems::Starfield::operator()(Wrecks::ECS::Entity entity)
{
    static std::mt19937 s_Random{std::random_device{}()};

    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    auto& transform = engine.get<Components::Transform>()[entity];
    const auto& star = engine.get<Components::Star>()[entity];

    transform.x -= star.speed * *m_DeltaTime;

    if (transform.x < -star.size) {
        const RType::Render::Vec2 screen = m_Window->virtualSize();

        transform.x += screen.x + star.size;
        transform.y = std::uniform_real_distribution<float>(0.0f, screen.y)(s_Random);
    }
}

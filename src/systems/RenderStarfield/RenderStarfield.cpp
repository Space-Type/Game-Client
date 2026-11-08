/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** RenderStarfield system
*/

#include "RenderStarfield.hpp"
#include <rtype/game.hpp>


RType::Systems::RenderStarfield::RenderStarfield() noexcept
    : m_Engine(nullptr)
    , m_Renderer(nullptr)
{

}

void RType::Systems::RenderStarfield::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    if (!m_Renderer)
        return;

    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    const auto& star = engine.get<Components::Star>()[entity];

    m_Renderer->drawRect({ .x = star.position.x, .y = star.position.y, .width = star.size, .height = star.size }, star.color);
}

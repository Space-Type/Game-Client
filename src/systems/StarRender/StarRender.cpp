/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** StarRender System
*/

#include "StarRender.hpp"
#include <rtype/game.hpp>


RType::Systems::StarRender::StarRender() noexcept
    : m_Engine(nullptr)
    , m_Renderer(nullptr)
{

}

void RType::Systems::StarRender::operator()(Wrecks::ECS::Entity entity)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    const auto& transform = engine.get<Protecs::Components::Transform>()[entity];
    const auto& star = engine.get<Components::Star>()[entity];

    m_Renderer->drawRect({.x = transform.position.x, .y = transform.position.y, .width = star.size, .height = star.size}, star.color);
}

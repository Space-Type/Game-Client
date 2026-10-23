/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Render System
*/

#include "Render.hpp"
#include <rtype/game.hpp>
#include <rtype/render.hpp>


RType::Systems::Render::Render() noexcept
    : m_Engine(nullptr)
    , m_Renderer(nullptr)
{

}

void RType::Systems::Render::operator()(Wrecks::ECS::Entity entity)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    auto& transform = engine.get<Protecs::Components::Transform>()[entity];
    auto& sprite = engine.get<Components::Sprite>()[entity];

    m_Renderer->drawSprite(sprite.texture, {.position = { transform.position.x, transform.position.y }});
}

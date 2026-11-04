/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** RenderSprite System
*/

#include "RenderSprite.hpp"
#include <rtype/game.hpp>
#include <rtype/render.hpp>


RType::Systems::RenderSprite::RenderSprite() noexcept
    : m_Engine(nullptr)
    , m_Renderer(nullptr)
{

}

void RType::Systems::RenderSprite::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    auto& transform = engine.get<Protecs::Components::Transform>()[entity];
    auto& sprite = engine.get<Components::Sprite>()[entity];

    float scale = transform.scale * Game::Game::WORLD_TO_SCREEN;

    m_Renderer->drawSprite(sprite.texture, {
        .position = transform.position * Game::Game::WORLD_TO_SCREEN,
        .rotation = transform.rotation,
        .scale = { .x = scale, .y = scale },
    });
}

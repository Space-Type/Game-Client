/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Render System
*/

#include "Render.hpp"
#include <rtype/game.hpp>
#include <raylib.h>


RType::Systems::Render::Render() noexcept
{

}

void RType::Systems::Render::operator()(Wrecks::ECS::Entity entity)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    auto& transform = engine.get<Components::Transform>()[entity];
    auto& sprite = engine.get<Components::Sprite>()[entity];

    DrawTexture(sprite.texture, transform.x, transform.y, WHITE);
}

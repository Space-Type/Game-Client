/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** RenderSprite system
*/

#include "RenderSprite.hpp"
#include <rtype/game.hpp>
#include <rtype/render.hpp>
#include <chrono>


RType::Systems::RenderSprite::RenderSprite() noexcept
    : m_Engine(nullptr)
    , m_Renderer(nullptr)
{

}

void RType::Systems::RenderSprite::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt)
{
    using Protecs::Networking::Packets::EntityCreate;

    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    auto& transform = engine.get<Protecs::Components::Transform>()[entity];
    auto& sprite = engine.get<Components::Sprite>()[entity];
    float seconds = std::chrono::duration<float>(dt).count();

    sprite.elapsed += seconds;
    if (Components::isEliminated(engine, entity))
        return;
    if (engine.hasComponent<Components::Blink>(entity)) {
        auto& blink = engine.get<Components::Blink>()[entity];

        blink.remaining -= seconds;
        if (!Components::blinkVisible(blink.remaining))
            return;
    }

    std::size_t frame = Components::frameIndex(sprite);
    bool isPlayer = engine.hasComponent<Components::EntityKind>(entity)
                 && engine.get<Components::EntityKind>()[entity].type == EntityCreate::PLAYER;

    if (isPlayer && engine.hasComponent<Protecs::Components::Velocity>(entity))
        frame = tiltFrame(engine.get<Protecs::Components::Velocity>()[entity].position.y);

    float scale = transform.scale * Game::Game::WORLD_TO_SCREEN;

    m_Renderer->drawSprite(sprite.texture, {
        .position = transform.position * Game::Game::WORLD_TO_SCREEN,
        .source = Components::frameAt(sprite, frame),
        .rotation = transform.rotation,
        .scale = { .x = scale, .y = scale },
    });
}

auto RType::Systems::spriteSize(const Components::Sprite& sprite, const Render::Resources& resources) noexcept -> Render::Vec2D
{
    if (sprite.frame)
        return { .x = sprite.frame->width, .y = sprite.frame->height };
    return resources.textureSize(sprite.texture);
}

std::size_t RType::Systems::tiltFrame(float velocityY) noexcept
{
    if (velocityY < 0.0F)
        return RenderSprite::TILT_UP_FRAME;
    if (velocityY > 0.0F)
        return RenderSprite::TILT_DOWN_FRAME;
    return RenderSprite::STILL_FRAME;
}

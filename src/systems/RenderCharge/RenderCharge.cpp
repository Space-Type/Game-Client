/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** RenderCharge system
*/

#include "RenderCharge.hpp"
#include "../RenderSprite/RenderSprite.hpp"
#include "../../game/SpriteSheets/SpriteSheets.hpp"
#include <rtype/game.hpp>
#include <chrono>


RType::Systems::RenderCharge::RenderCharge() noexcept
    : m_Engine(nullptr)
    , m_Renderer(nullptr)
    , m_Resources(nullptr)
    , m_Elapsed(0.0F)
{

}

void RType::Systems::RenderCharge::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    m_Elapsed += std::chrono::duration<float>(dt).count();
    if (Components::isEliminated(engine, entity))
        return;

    std::uint8_t level = engine.hasComponent<Components::LocalCharge>(entity)
                       ? Components::chargeLevel(engine.get<Components::LocalCharge>()[entity].seconds)
                       : engine.get<Protecs::Components::PlayerStats>()[entity].charge;

    if (level == 0)
        return;

    auto& transform = engine.get<Protecs::Components::Transform>()[entity];
    float scale = transform.scale * Game::Game::WORLD_TO_SCREEN;
    Render::Vec2D ship = spriteSize(engine.get<Components::Sprite>()[entity], *m_Resources);
    Components::Sprite charge = Game::chargeSprite(*m_Resources);

    charge.elapsed = m_Elapsed;

    Render::Vec2D size = spriteSize(charge, *m_Resources);
    Render::Vec2D position = transform.position * Game::Game::WORLD_TO_SCREEN;

    // In front of the ship, centered on its height
    m_Renderer->drawSprite(charge.texture, {
        .position = { .x = position.x + ship.x * scale, .y = position.y + (ship.y - size.y) * scale / 2.0F },
        .source = Components::frameAt(charge, Components::frameIndex(charge)),
        .scale = { .x = scale, .y = scale },
    });
}

/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** RenderOutline system
*/

#include "RenderOutline.hpp"
#include "../RenderSprite/RenderSprite.hpp"
#include "../../ui/Theme.hpp"
#include <rtype/game.hpp>


RType::Systems::RenderOutline::RenderOutline() noexcept
    : m_Engine(nullptr)
    , m_Renderer(nullptr)
    , m_Resources(nullptr)
    , m_Options(nullptr)
{

}

void RType::Systems::RenderOutline::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    using Protecs::Networking::Packets::EntityCreate;

    if (!m_Options || !m_Options->highContrast)
        return;

    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    EntityCreate::EntityType type = engine.get<Components::EntityKind>()[entity].type;

    if (type == EntityCreate::GENERIC)
        return;

    auto& transform = engine.get<Protecs::Components::Transform>()[entity];
    float scale = transform.scale * Game::Game::WORLD_TO_SCREEN;
    Render::Vec2D size = { .x = DEFAULT_SIZE, .y = DEFAULT_SIZE };

    if (engine.hasComponent<Components::Sprite>(entity))
        size = spriteSize(engine.get<Components::Sprite>()[entity], *m_Resources);

    // Same box as RenderSprite: the position is the top-left corner
    Render::Vec2D position = transform.position * Game::Game::WORLD_TO_SCREEN;

    m_Renderer->drawRectLines({ .x = position.x, .y = position.y, .width = size.x * scale, .height = size.y * scale },
                              UI::Theme::FOCUS_THICKNESS,
                              type == EntityCreate::PLAYER ? UI::Theme::FOCUS_COLOR : UI::Theme::ERROR_COLOR);
}

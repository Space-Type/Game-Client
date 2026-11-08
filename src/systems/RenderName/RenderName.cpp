/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** RenderName system
*/

#include "RenderName.hpp"
#include "../RenderOutline/RenderOutline.hpp"
#include "../RenderSprite/RenderSprite.hpp"
#include "../../ui/Theme.hpp"
#include <rtype/game.hpp>


namespace {
    // Dark plate behind the name: readable over any background
    constexpr RType::Render::Color PLATE_COLOR = { .r = 20, .g = 20, .b = 30, .a = 200 };
}


RType::Systems::RenderName::RenderName() noexcept
    : m_Engine(nullptr)
    , m_Renderer(nullptr)
    , m_Resources(nullptr)
{

}

void RType::Systems::RenderName::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    std::string name = printableName(engine.get<Protecs::Components::PlayerName>()[entity].str());

    if (name.empty() || Components::isEliminated(engine, entity))
        return;

    auto& transform = engine.get<Protecs::Components::Transform>()[entity];
    float shipWidth = RenderOutline::DEFAULT_SIZE;

    if (engine.hasComponent<Components::Sprite>(entity))
        shipWidth = spriteSize(engine.get<Components::Sprite>()[entity], *m_Resources).x;
    shipWidth *= transform.scale * Game::Game::WORLD_TO_SCREEN;

    // Centered above the ship, whose position is its top-left corner (see RenderSprite)
    Render::Vec2D ship = transform.position * Game::Game::WORLD_TO_SCREEN;
    Render::Vec2D size = m_Renderer->measureText(name, TEXT_SIZE);
    Render::Vec2D text = { .x = ship.x + (shipWidth - size.x) / 2.0F, .y = ship.y - MARGIN * 2.0F - size.y };

    m_Renderer->drawRect({ .x = text.x - MARGIN, .y = text.y - MARGIN, .width = size.x + MARGIN * 2.0F, .height = size.y + MARGIN * 2.0F },
                         PLATE_COLOR);
    m_Renderer->drawText(name, text, TEXT_SIZE, UI::Theme::FOREGROUND);
}

std::string RType::Systems::printableName(std::string_view name)
{
    std::string result(name);

    for (char& c : result)
        if (c < ' ' || c > '~')
            c = '?';
    return result;
}

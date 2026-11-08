/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** RenderPopup system
*/

#include "RenderPopup.hpp"
#include <rtype/game.hpp>
#include <cstdint>


RType::Systems::RenderPopup::RenderPopup() noexcept
    : m_Engine(nullptr)
    , m_Renderer(nullptr)
{

}

void RType::Systems::RenderPopup::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    const auto& popup = engine.get<Components::Popup>()[entity];
    float remaining = engine.get<Components::Lifetime>()[entity].remaining;
    Render::Vec2D at = engine.get<Protecs::Components::Transform>()[entity].position * Game::Game::WORLD_TO_SCREEN;
    Render::Vec2D size = m_Renderer->measureText(popup.text, TEXT_SIZE);
    Render::Color color = popup.color;

    color.a = static_cast<std::uint8_t>(static_cast<float>(color.a) * Components::popupOpacity(remaining, popup.duration));
    m_Renderer->drawText(popup.text, { .x = at.x - size.x / 2.0F, .y = at.y - size.y - Components::popupRise(remaining, popup.duration) },
                         TEXT_SIZE, color);
}

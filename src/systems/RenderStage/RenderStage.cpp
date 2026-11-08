/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** RenderStage system
*/

#include "RenderStage.hpp"
#include <rtype/game.hpp>
#include <algorithm>
#include <string>


RType::Systems::RenderStage::RenderStage() noexcept
    : m_Engine(nullptr)
    , m_Renderer(nullptr)
    , m_Resources(nullptr)
{

}

void RType::Systems::RenderStage::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    const auto& level = engine.get<Protecs::Components::Level>()[entity];

    if (level.stage < 1 || level.stage > STAGES)
        return;

    Render::TextureId background;

    try {
        background = m_Resources->loadTexture(Render::assetPath("stages/stage-" + std::to_string(level.stage) + ".png"),
                                              Render::Colors::BLACK);
    } catch (const std::exception&) {
        return;  // no background: the starfield alone
    }

    constexpr Render::Vec2D SCREEN = { .x = static_cast<float>(Game::Game::VIRTUAL_WIDTH), .y = static_cast<float>(Game::Game::VIRTUAL_HEIGHT) };
    Render::Vec2D size = m_Resources->textureSize(background);
    float scale = SCREEN.y / size.y;

    m_Renderer->drawSprite(background, {
        .position = { .x = 0.0F, .y = 0.0F },
        .source = stageView(static_cast<float>(level.progression), size, SCREEN),
        .scale = { .x = scale, .y = scale },
    });
}

auto RType::Systems::stageView(float progression, Render::Vec2D texture, Render::Vec2D screen) noexcept -> Render::Rect
{
    if (texture.y <= 0.0F || screen.y <= 0.0F)
        return {};

    float visible = std::min(texture.x, screen.x * texture.y / screen.y);

    return { .x = std::clamp(progression, 0.0F, texture.x - visible), .y = 0.0F, .width = visible, .height = texture.y };
}

/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** InGame scene
*/

#include "InGame.hpp"
#include "../Text.hpp"
#include <chrono>


auto RType::Scenes::InGame::update(Context& ctx, Wrecks::DeltaTime dt) -> std::optional<SceneId>
{
    m_Seconds += std::chrono::duration<float>(dt).count();
    if (ctx.input.isPressed(Render::Key::Escape) || ctx.input.isPressed(ctx.controls.gamepad(), Render::GamepadButton::Start))
        return SceneId::Paused;
    return std::nullopt;
}

void RType::Scenes::InGame::render(Context& ctx) const
{
    if (!ctx.session)
        return;

    Render::TextureId heart = ctx.resources.loadTexture(Render::assetPath("heart.png"));

    UI::Hud::draw(ctx.renderer, ctx.resources, heart, ctx.session->players(), m_Seconds);
}

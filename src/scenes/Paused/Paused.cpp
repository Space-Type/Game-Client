/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Paused scene
*/

#include "Paused.hpp"
#include "../Text.hpp"


namespace {
    constexpr RType::Render::Color OVERLAY = { .r = 0, .g = 0, .b = 0, .a = 160 };
}


auto RType::Scenes::Paused::update(Context& ctx, Wrecks::DeltaTime) -> std::optional<SceneId>
{
    int gamepad = ctx.controls.gamepad();

    if (ctx.input.isPressed(Render::Key::Enter) || ctx.input.isPressed(gamepad, Render::GamepadButton::A))
        return SceneId::InGame;
    if (ctx.input.isPressed(Render::Key::Backspace) || ctx.input.isPressed(gamepad, Render::GamepadButton::Y)) {
        ctx.session.reset();  // the client disconnects on destruction
        return SceneId::Menu;
    }
    return std::nullopt;
}

void RType::Scenes::Paused::render(Context& ctx) const
{
    Render::Vec2D size = ctx.window.virtualSize();

    ctx.renderer.drawRect({ .x = 0.0F, .y = 0.0F, .width = size.x, .height = size.y }, OVERLAY);
    drawCenteredText(ctx.renderer, ctx.window, "Paused", 200.0F, TITLE_SIZE);
    drawCenteredText(ctx.renderer, ctx.window, "Enter / A: Resume", 380.0F, TEXT_SIZE);
    drawCenteredText(ctx.renderer, ctx.window, "Backspace / Y: Disconnect", 440.0F, TEXT_SIZE);
}

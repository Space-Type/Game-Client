/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Menu scene
*/

#include "Menu.hpp"
#include "../Text.hpp"


auto RType::Scenes::Menu::update(Context& ctx, Wrecks::DeltaTime) -> std::optional<SceneId>
{
    int gamepad = ctx.controls.gamepad();

    if (ctx.input.isPressed(Render::Key::Enter) || ctx.input.isPressed(gamepad, Render::GamepadButton::A))
        return SceneId::Connecting;
    if (ctx.input.isPressed(Render::Key::Escape) || ctx.input.isPressed(gamepad, Render::GamepadButton::B))
        return SceneId::Quit;
    return std::nullopt;
}

void RType::Scenes::Menu::render(Context& ctx) const
{
    drawCenteredText(ctx.renderer, ctx.window, "R-Type", 200.0F, TITLE_SIZE);
    drawCenteredText(ctx.renderer, ctx.window, "Enter / A: Play", 380.0F, TEXT_SIZE);
    drawCenteredText(ctx.renderer, ctx.window, "Escape / B: Quit", 440.0F, TEXT_SIZE);
}

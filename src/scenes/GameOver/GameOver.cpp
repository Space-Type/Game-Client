/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** GameOver scene
*/

#include "GameOver.hpp"
#include "../Text.hpp"


auto RType::Scenes::GameOver::update(Context& ctx, Wrecks::DeltaTime) -> std::optional<SceneId>
{
    if (ctx.input.isPressed(Render::Key::Enter) || ctx.input.isPressed(ctx.controls.gamepad(), Render::GamepadButton::A)) {
        ctx.message.clear();
        return SceneId::Menu;
    }
    return std::nullopt;
}

void RType::Scenes::GameOver::render(Context& ctx) const
{
    Wrecks::cstring_view message = ctx.message.empty() ? Wrecks::cstring_view("Game over") : Wrecks::cstring_view(ctx.message);

    drawCenteredText(ctx.renderer, ctx.window, message, 300.0F, TEXT_SIZE);
    drawCenteredText(ctx.renderer, ctx.window, "Enter / A: Back to menu", 400.0F, TEXT_SIZE);
}

/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** GameOver scene
*/

#include "GameOver.hpp"
#include "../Text.hpp"
#include <string>


auto RType::Scenes::GameOver::update(Context& ctx, Wrecks::DeltaTime) -> std::optional<SceneId>
{
    if (ctx.input.isPressed(Render::Key::Enter) || ctx.input.isPressed(ctx.controls.gamepad(), Render::GamepadButton::A)) {
        ctx.message.clear();
        ctx.outcome = Game::Outcome::Message;
        return SceneId::Connect;
    }
    return std::nullopt;
}

void RType::Scenes::GameOver::render(Context& ctx) const
{
    switch (ctx.outcome) {
        case Game::Outcome::Victory:
            drawResult(ctx, "Victory", { .r = 90, .g = 220, .b = 100, .a = 255 });
            break;
        case Game::Outcome::Defeat:
            drawResult(ctx, "Game over", { .r = 235, .g = 60, .b = 60, .a = 255 });
            break;
        case Game::Outcome::Message:
            drawCenteredText(ctx.renderer, ctx.window, ctx.message.empty() ? Wrecks::cstring_view("Game over") : Wrecks::cstring_view(ctx.message),
                             300.0F, TEXT_SIZE);
            break;
    }
    drawCenteredText(ctx.renderer, ctx.window, "Enter / A: Back to menu", 420.0F, TEXT_SIZE);
}

// The title in big coloured letters, the score below it
void RType::Scenes::GameOver::drawResult(const Context& ctx, Wrecks::cstring_view title, Render::Color color)
{
    std::string score = "Score " + std::to_string(ctx.score);

    drawCenteredText(ctx.renderer, ctx.window, title, 220.0F, TITLE_SIZE * 1.5F, color);
    drawCenteredText(ctx.renderer, ctx.window, score, 330.0F, TEXT_SIZE);
}

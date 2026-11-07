/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** GameOver scene
*/

#include "GameOver.hpp"
#include "../Text.hpp"
#include "../../ui/Theme.hpp"
#include <string>


auto RType::Scenes::GameOver::update(Context& ctx, Wrecks::DeltaTime) -> std::optional<SceneId>
{
    if (ctx.input.isPressed(Render::Key::Enter) || ctx.input.isPressed(ctx.controls.gamepad(), Render::GamepadButton::A)) {
        ctx.message.clear();
        return SceneId::Connect;
    }
    return std::nullopt;
}

/**
 * A message "title\nscore" shows its title in big letters
 * ("Victory" in green) above the score; any other message
 * (disconnection, network error) is shown as is.
 */
void RType::Scenes::GameOver::render(Context& ctx) const
{
    constexpr float HINT_Y = 420.0F;
    std::size_t split = ctx.message.find('\n');

    if (split == std::string::npos) {
        Wrecks::cstring_view message = ctx.message.empty() ? Wrecks::cstring_view("Game over") : Wrecks::cstring_view(ctx.message);

        drawCenteredText(ctx.renderer, ctx.window, message, 300.0F, TEXT_SIZE);
    } else {
        std::string title = ctx.message.substr(0, split);
        std::string detail = ctx.message.substr(split + 1);
        Render::Color color = title == "Victory" ? UI::Theme::VICTORY_COLOR : UI::Theme::FOREGROUND;

        drawCenteredText(ctx.renderer, ctx.window, title, 220.0F, TITLE_SIZE * 1.5F, color);
        drawCenteredText(ctx.renderer, ctx.window, detail, 330.0F, TEXT_SIZE);
    }
    drawCenteredText(ctx.renderer, ctx.window, "Enter / A: Back to menu", HINT_Y, TEXT_SIZE);
}

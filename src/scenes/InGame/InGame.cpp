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

    if (auto boss = ctx.session->bossHealth())
        UI::Hud::drawBossBar(ctx.renderer, ctx.window.virtualSize(), boss->hp, boss->max);

    if (auto shown = ctx.session->announcement())
        drawAnnouncement(ctx, shown->first, shown->second);
}

/**
 * "LEVEL n" or "STAGE CLEAR" in big letters at the middle of
 * the screen, fading in and out; "STAGE CLEAR" also lists
 * every player's score.
 */
void RType::Scenes::InGame::drawAnnouncement(Context& ctx, const Game::Announcement& announcement, float elapsed)
{
    constexpr float TEXT_Y = 250.0F;
    constexpr float SCORES_Y = 350.0F;
    constexpr float SCORE_STEP = 40.0F;

    float opacity = UI::Hud::announcementOpacity(elapsed, announcement.duration);
    auto alpha = static_cast<std::uint8_t>(255.0F * opacity);
    Render::Color title = UI::Theme::FOCUS_COLOR;
    Render::Color text = UI::Theme::FOREGROUND;

    title.a = alpha;
    text.a = alpha;
    drawCenteredText(ctx.renderer, ctx.window, announcement.text, TEXT_Y, TITLE_SIZE * 1.5F, title);
    if (!announcement.scores)
        return;

    float y = SCORES_Y;

    for (const auto& line : ctx.session->players()) {
        std::string row = line.name + "   " + UI::Hud::scoreText(line.score);

        drawCenteredText(ctx.renderer, ctx.window, row, y, TEXT_SIZE, text);
        y += SCORE_STEP;
    }
}

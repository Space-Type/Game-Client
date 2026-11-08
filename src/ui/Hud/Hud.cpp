/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** in-game HUD
*/

#include "Hud.hpp"
#include "../Theme.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>


namespace {
    // Dark plate behind the HUD: readable over the game without hiding much of it
    constexpr RType::Render::Color PLATE_COLOR = { .r = 20, .g = 20, .b = 30, .a = 160 };
    constexpr float HEART_GAP = 2.0F;
    constexpr const char* OUT_TEXT = "OUT";

    // Characters the default font cannot draw become '?', long names are cut
    std::string displayName(const std::string& name)
    {
        std::string result = name.substr(0, RType::UI::Hud::NAME_MAX_LENGTH);

        for (char& c : result)
            if (c < ' ' || c > '~')
                c = '?';
        return result;
    }
}


void RType::UI::Hud::draw(const Render::Renderer& renderer, const Render::Resources& resources, Render::TextureId heart,
                          const std::vector<HudLine>& lines, float seconds)
{
    if (lines.empty())
        return;

    Render::Vec2D heartSize = resources.textureSize(heart);
    float heartWidth = heartSize.x * HEART_SCALE;
    float markerWidth = renderer.measureText(Theme::FOCUS_MARKER, TEXT_SIZE).x;
    float nameWidth = 0.0F;
    float scoreWidth = renderer.measureText(scoreText(0), TEXT_SIZE).x;
    float heartsWidth = std::max(heartWidth + HEART_GAP, renderer.measureText(OUT_TEXT, TEXT_SIZE).x);

    for (const HudLine& line : lines) {
        float lineHearts = static_cast<float>(heartCount(line.lives)) * (heartWidth + HEART_GAP)
                         + renderer.measureText(livesLabel(line.lives), TEXT_SIZE).x;

        nameWidth = std::max(nameWidth, renderer.measureText(displayName(line.name), TEXT_SIZE).x);
        heartsWidth = std::max(heartsWidth, lineHearts);
    }

    float nameX = ORIGIN + PADDING + markerWidth + PADDING;
    float heartsX = nameX + nameWidth + GAP;
    float scoreX = heartsX + heartsWidth + GAP;

    renderer.drawRect({ .x = ORIGIN, .y = ORIGIN, .width = scoreX + scoreWidth + PADDING - ORIGIN,
                        .height = ROW_HEIGHT * static_cast<float>(lines.size()) + PADDING }, PLATE_COLOR);

    for (std::size_t row = 0; row < lines.size(); ++row) {
        const HudLine& line = lines[row];
        float y = ORIGIN + PADDING / 2.0F + ROW_HEIGHT * static_cast<float>(row);
        float textY = y + (ROW_HEIGHT - TEXT_SIZE) / 2.0F;

        if (line.local)
            renderer.drawText(Theme::FOCUS_MARKER, { .x = ORIGIN + PADDING, .y = textY }, TEXT_SIZE, Theme::FOCUS_COLOR);
        renderer.drawText(displayName(line.name), { .x = nameX, .y = textY }, TEXT_SIZE, Theme::FOREGROUND);

        if (line.lives == 0) {
            renderer.drawText(OUT_TEXT, { .x = heartsX, .y = textY }, TEXT_SIZE, Theme::BORDER_COLOR);
        } else if (heartsVisible(line.lives, seconds)) {
            std::uint8_t count = heartCount(line.lives);

            for (std::uint8_t i = 0; i < count; ++i)
                renderer.drawSprite(heart, {
                    .position = { .x = heartsX + static_cast<float>(i) * (heartWidth + HEART_GAP),
                                  .y = y + (ROW_HEIGHT - heartSize.y * HEART_SCALE) / 2.0F },
                    .scale = { .x = HEART_SCALE, .y = HEART_SCALE },
                    .tint = heartColor(line.lives),
                });
            renderer.drawText(livesLabel(line.lives), { .x = heartsX + static_cast<float>(count) * (heartWidth + HEART_GAP), .y = textY },
                              TEXT_SIZE, Theme::FOREGROUND);
        }
        renderer.drawText(scoreText(line.score), { .x = scoreX, .y = textY }, TEXT_SIZE, Theme::FOREGROUND);
    }
}

auto RType::UI::Hud::heartColor(std::uint8_t lives) noexcept -> Render::Color
{
    if (lives >= 3)
        return Theme::HEART_HIGH;
    if (lives == 2)
        return Theme::HEART_MEDIUM;
    return Theme::HEART_LOW;
}

std::uint8_t RType::UI::Hud::heartCount(std::uint8_t lives) noexcept
{
    return lives > HEART_STACK_LIMIT ? 1 : lives;
}

std::string RType::UI::Hud::livesLabel(std::uint8_t lives)
{
    if (lives <= HEART_STACK_LIMIT)
        return {};
    return "x" + std::to_string(lives);
}

bool RType::UI::Hud::heartsVisible(std::uint8_t lives, float seconds) noexcept
{
    if (lives != 1)
        return true;
    return std::fmod(seconds, LAST_LIFE_BLINK_PERIOD) < LAST_LIFE_BLINK_PERIOD * 0.7F;  // mostly shown
}

std::string RType::UI::Hud::scoreText(std::uint32_t score)
{
    char text[16];

    std::snprintf(text, sizeof(text), "%06u", static_cast<unsigned int>(score));
    return text;
}

std::string RType::UI::Hud::bossLabel(std::string_view name, std::uint16_t hp, std::uint16_t max)
{
    return std::string(name) + "  " + std::to_string(hp) + " / " + std::to_string(max);
}

void RType::UI::Hud::drawBossBar(const Render::Renderer& renderer, Render::Vec2D screen, std::string_view name, std::uint16_t hp, std::uint16_t max)
{
    std::string label = bossLabel(name, hp, max);
    float labelWidth = renderer.measureText(label, TEXT_SIZE).x;
    float x = (screen.x - BOSS_BAR_WIDTH) / 2.0F;
    float barY = screen.y - ORIGIN - BOSS_BAR_HEIGHT;
    float labelY = barY - PADDING - TEXT_SIZE;

    renderer.drawRect({ .x = x - PADDING, .y = labelY - PADDING, .width = BOSS_BAR_WIDTH + PADDING * 2.0F,
                        .height = barY + BOSS_BAR_HEIGHT + PADDING - (labelY - PADDING) }, PLATE_COLOR);
    renderer.drawText(label, { .x = (screen.x - labelWidth) / 2.0F, .y = labelY }, TEXT_SIZE, Theme::FOREGROUND);
    renderer.drawRect({ .x = x, .y = barY, .width = BOSS_BAR_WIDTH, .height = BOSS_BAR_HEIGHT }, Theme::TRACK_COLOR);
    renderer.drawRect({ .x = x, .y = barY, .width = bossBarFill(hp, max, BOSS_BAR_WIDTH), .height = BOSS_BAR_HEIGHT }, Theme::HEART_LOW);
}

float RType::UI::Hud::bossBarFill(std::uint16_t hp, std::uint16_t max, float width) noexcept
{
    if (max == 0)
        return 0.0F;
    return width * static_cast<float>(std::min(hp, max)) / static_cast<float>(max);
}

float RType::UI::Hud::announcementOpacity(float elapsed, float duration) noexcept
{
    if (elapsed < 0.0F || elapsed > duration)
        return 0.0F;
    if (elapsed < FADE_IN)
        return elapsed / FADE_IN;
    if (elapsed > duration - FADE_OUT)
        return std::max(0.0F, (duration - elapsed) / FADE_OUT);
    return 1.0F;
}

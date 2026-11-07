/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** sprites of the subject's sprite sheets
*/

#include "SpriteSheets.hpp"
#include <array>
#include <string>


namespace {
    using RType::Render::Rect;

    // Frame positions measured in the sheets (assets/sprites)
    constexpr float SHIP_WIDTH = 33.0F;     // r-typesheet42: 5 frames per row, tilted down to up
    constexpr float SHIP_HEIGHT = 17.0F;
    constexpr std::size_t SHIP_FRAMES = 5;

    constexpr Rect ENEMY_FRAME = { .x = 0.0F, .y = 0.0F, .width = 33.0F, .height = 36.0F };  // r-typesheet5: 16 frames
    constexpr std::size_t ENEMY_FRAMES = 16;
    constexpr float ENEMY_FPS = 12.0F;

    constexpr Rect ENEMY_MISSILE_FRAME = { .x = 135.0F, .y = 5.0F, .width = 9.0F, .height = 8.0F };    // r-typesheet43

    // r-typesheet1: one row per charge level, 2 frames each (head on the right); the frame width includes a 1 px gap
    constexpr std::array<Rect, 5> BEAM_FRAMES = {{
        { .x = 232.0F, .y = 103.0F, .width = 17.0F, .height = 12.0F },
        { .x = 200.0F, .y = 120.0F, .width = 33.0F, .height = 12.0F },
        { .x = 168.0F, .y = 136.0F, .width = 49.0F, .height = 14.0F },
        { .x = 136.0F, .y = 154.0F, .width = 65.0F, .height = 14.0F },
        { .x = 104.0F, .y = 170.0F, .width = 81.0F, .height = 16.0F },
    }};
    constexpr float BEAM_FPS = 10.0F;

    constexpr Rect CHARGE_FRAME = { .x = 134.0F, .y = 18.0F, .width = 33.0F, .height = 32.0F };  // r-typesheet1: 8 frames
    constexpr std::size_t CHARGE_FRAMES = 8;
    constexpr float CHARGE_FPS = 16.0F;

    // r-typesheet30: last row of the Dobkeratops (chest open), 4 frames 162 px apart (155 px wide + gap)
    constexpr Rect BOSS_FRAME = { .x = 27.0F, .y = 1711.0F, .width = 162.0F, .height = 204.0F };
    constexpr std::size_t BOSS_FRAMES = 4;
    constexpr float BOSS_FPS = 4.0F;

    constexpr Rect EXPLOSION_FRAME = { .x = 129.0F, .y = 0.0F, .width = 33.0F, .height = 33.0F };      // r-typesheet44: 6 frames
    constexpr std::size_t EXPLOSION_FRAMES = 6;
    constexpr float EXPLOSION_FPS = 15.0F;

    RType::Render::TextureId sheet(RType::Render::Resources& resources, const char* name)
    {
        return resources.loadTexture(RType::Render::assetPath(std::string("sprites/") + name));
    }
}


auto RType::Game::playerSprite(Render::Resources& resources, std::size_t slot) -> Components::Sprite
{
    Components::Sprite sprite{
        .texture = sheet(resources, "r-typesheet42.gif"),
        .frame = Rect{ .x = 0.0F, .y = 0.0F, .width = SHIP_WIDTH, .height = SHIP_HEIGHT },
        .frameCount = SHIP_FRAMES,  // chosen by the tilt, not played
    };

    setPlayerColour(sprite, slot);
    return sprite;
}

auto RType::Game::enemySprite(Render::Resources& resources) -> Components::Sprite
{
    return { .texture = sheet(resources, "r-typesheet5.gif"), .frame = ENEMY_FRAME, .frameCount = ENEMY_FRAMES, .framesPerSecond = ENEMY_FPS };
}

auto RType::Game::playerMissileSprite(Render::Resources& resources) -> Components::Sprite
{
    return { .texture = resources.loadTexture(Render::assetPath("blueshoot.png")) };
}

auto RType::Game::enemyMissileSprite(Render::Resources& resources) -> Components::Sprite
{
    return { .texture = sheet(resources, "r-typesheet43.gif"), .frame = ENEMY_MISSILE_FRAME };
}

auto RType::Game::explosionSprite(Render::Resources& resources) -> Components::Sprite
{
    return {
        .texture = sheet(resources, "r-typesheet44.gif"),
        .frame = EXPLOSION_FRAME,
        .frameCount = EXPLOSION_FRAMES,
        .framesPerSecond = EXPLOSION_FPS,
        .loop = false,
    };
}

auto RType::Game::beamSprite(Render::Resources& resources, std::uint8_t level) -> Components::Sprite
{
    std::size_t index = level < 1 ? 0 : (level > BEAM_FRAMES.size() ? BEAM_FRAMES.size() - 1 : level - 1);

    return { .texture = sheet(resources, "r-typesheet1.gif"), .frame = BEAM_FRAMES[index], .frameCount = 2, .framesPerSecond = BEAM_FPS };
}

auto RType::Game::chargeSprite(Render::Resources& resources) -> Components::Sprite
{
    return { .texture = sheet(resources, "r-typesheet1.gif"), .frame = CHARGE_FRAME, .frameCount = CHARGE_FRAMES, .framesPerSecond = CHARGE_FPS };
}

auto RType::Game::bossSprite(Render::Resources& resources) -> Components::Sprite
{
    return { .texture = sheet(resources, "r-typesheet30.gif"), .frame = BOSS_FRAME, .frameCount = BOSS_FRAMES, .framesPerSecond = BOSS_FPS };
}

void RType::Game::setPlayerColour(Components::Sprite& sprite, std::size_t slot) noexcept
{
    if (sprite.frame)
        sprite.frame->y = SHIP_HEIGHT * static_cast<float>(slot % PLAYER_COLOURS);
}

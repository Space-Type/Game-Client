/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** sprites of the subject's sprite sheets
*/

#include "SpriteSheets.hpp"
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

void RType::Game::setPlayerColour(Components::Sprite& sprite, std::size_t slot) noexcept
{
    if (sprite.frame)
        sprite.frame->y = SHIP_HEIGHT * static_cast<float>(slot % PLAYER_COLOURS);
}

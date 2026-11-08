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

    // r-typesheet30a: a spinning ball, 3 frames: a segment of the Dobkeratops tail
    constexpr Rect TAIL_FRAME = { .x = 0.0F, .y = 0.0F, .width = 34.0F, .height = 34.0F };
    constexpr std::size_t TAIL_FRAMES = 3;
    constexpr float TAIL_FPS = 10.0F;

    // r-typesheet32: the Gomander with its eye open (last row), 2 frames 260 px apart; its hitboxes are on the server
    constexpr Rect GOMANDER_FRAME = { .x = 2.0F, .y = 431.0F, .width = 260.0F, .height = 140.0F };
    constexpr std::size_t GOMANDER_FRAMES = 2;
    constexpr float GOMANDER_FPS = 3.0F;

    constexpr Rect EXPLOSION_FRAME = { .x = 129.0F, .y = 0.0F, .width = 33.0F, .height = 33.0F };      // r-typesheet44: 6 frames
    constexpr std::size_t EXPLOSION_FRAMES = 6;
    constexpr float EXPLOSION_FPS = 15.0F;

    // r-typesheet7: the POW Armor walking, 6 frames 33 px apart
    constexpr Rect POW_ARMOR_FRAME = { .x = 1.0F, .y = 1.0F, .width = 33.0F, .height = 33.0F };
    constexpr std::size_t POW_ARMOR_FRAMES = 6;
    constexpr float POW_ARMOR_FPS = 8.0F;

    // r-typesheet3: the Force item spinning, 12 frames 17 px apart
    constexpr Rect FORCE_PICKUP_FRAME = { .x = 1.0F, .y = 1.0F, .width = 17.0F, .height = 16.0F };
    constexpr std::size_t FORCE_PICKUP_FRAMES = 12;
    constexpr float FORCE_PICKUP_FPS = 12.0F;

    // r-typesheet2: the Force by level; the frames are its hitbox on the server (Rules::FORCE_SIZES)
    struct ForceFrames {
        Rect frame;
        std::size_t count;
    };
    constexpr std::array<ForceFrames, 3> FORCE_FRAMES = {{
        { .frame = { .x = 212.0F, .y = 69.0F, .width = 30.0F, .height = 22.0F }, .count = 2 },  // an orb with two arms
        { .frame = { .x = 164.0F, .y = 377.0F, .width = 34.0F, .height = 28.0F }, .count = 2 }, // with spikes
        { .frame = { .x = 173.0F, .y = 342.0F, .width = 32.0F, .height = 32.0F }, .count = 3 }, // the big spiked one
    }};
    constexpr float FORCE_FPS = 10.0F;

    // heart.png is a white outline (the HUD tints it too): a red heart, its shape tells what it is
    constexpr RType::Render::Color LIFE_PICKUP_TINT = { .r = 235, .g = 60, .b = 60, .a = 255 };

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

auto RType::Game::tailSprite(Render::Resources& resources) -> Components::Sprite
{
    return { .texture = sheet(resources, "r-typesheet30a.gif"), .frame = TAIL_FRAME, .frameCount = TAIL_FRAMES, .framesPerSecond = TAIL_FPS };
}

auto RType::Game::lifePickupSprite(Render::Resources& resources) -> Components::Sprite
{
    return { .texture = resources.loadTexture(Render::assetPath("heart.png")), .tint = LIFE_PICKUP_TINT };
}

auto RType::Game::powArmorSprite(Render::Resources& resources) -> Components::Sprite
{
    return { .texture = sheet(resources, "r-typesheet7.gif"), .frame = POW_ARMOR_FRAME, .frameCount = POW_ARMOR_FRAMES, .framesPerSecond = POW_ARMOR_FPS };
}

auto RType::Game::forcePickupSprite(Render::Resources& resources) -> Components::Sprite
{
    return { .texture = sheet(resources, "r-typesheet3.gif"), .frame = FORCE_PICKUP_FRAME, .frameCount = FORCE_PICKUP_FRAMES,
             .framesPerSecond = FORCE_PICKUP_FPS };
}

auto RType::Game::forceSprite(Render::Resources& resources, std::uint8_t level) -> Components::Sprite
{
    const auto& frames = FORCE_FRAMES[level >= 1 && level <= FORCE_FRAMES.size() ? level - 1 : 0];

    return { .texture = sheet(resources, "r-typesheet2.gif"), .frame = frames.frame, .frameCount = frames.count, .framesPerSecond = FORCE_FPS };
}

auto RType::Game::gomanderSprite(Render::Resources& resources) -> Components::Sprite
{
    return { .texture = sheet(resources, "r-typesheet32.gif"), .frame = GOMANDER_FRAME, .frameCount = GOMANDER_FRAMES, .framesPerSecond = GOMANDER_FPS };
}

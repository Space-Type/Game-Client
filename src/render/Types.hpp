/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Render value types
*/

#pragma once

#include <cstdint>
#include <optional>

namespace RType::Render {

    /////////////////////////////////////////////////////////
    //                                                     //
    //                     VALUE TYPES                     //
    //                                                     //
    /////////////////////////////////////////////////////////

    struct Vec2 {
        float x = 0.0F;
        float y = 0.0F;

        constexpr bool operator==(const Vec2&) const noexcept = default;
        constexpr Vec2 operator+(Vec2 other) const noexcept { return {x + other.x, y + other.y}; }
        constexpr Vec2 operator-(Vec2 other) const noexcept { return {x - other.x, y - other.y}; }
        constexpr Vec2 operator*(float factor) const noexcept { return {x * factor, y * factor}; }
    };

    struct Rect {
        float x = 0.0F;
        float y = 0.0F;
        float width = 0.0F;
        float height = 0.0F;

        constexpr bool operator==(const Rect&) const noexcept = default;
    };

    struct Color {
        std::uint8_t r = 0;
        std::uint8_t g = 0;
        std::uint8_t b = 0;
        std::uint8_t a = 255;

        constexpr bool operator==(const Color&) const noexcept = default;
    };

    namespace Colors {
        inline constexpr Color BLACK = {0, 0, 0, 255};
        inline constexpr Color WHITE = {255, 255, 255, 255};
        inline constexpr Color GRAY = {130, 130, 130, 255};
        inline constexpr Color RED = {230, 41, 55, 255};
        inline constexpr Color GREEN = {0, 228, 48, 255};
        inline constexpr Color BLUE = {0, 121, 241, 255};
        inline constexpr Color YELLOW = {253, 249, 0, 255};
        inline constexpr Color ORANGE = {255, 161, 0, 255};
        inline constexpr Color PURPLE = {200, 122, 255, 255};
        inline constexpr Color INVISIBLE = {0, 0, 0, 0};  // Not TRANSPARENT: macro in wingdi.h
    }


    /////////////////////////////////////////////////////////
    //                                                     //
    //                   RESOURCE HANDLES                  //
    //                                                     //
    /////////////////////////////////////////////////////////

    // Handles are plain values, safe to store in ECS components.
    // Valid handles start at 1; the owning manager maps them to
    // the underlying library objects.
    enum class TextureId : std::uint32_t { Invalid = 0 };
    enum class FontId : std::uint32_t { Default = 0 };  // Default = library built-in font
    enum class SoundId : std::uint32_t { Invalid = 0 };
    enum class MusicId : std::uint32_t { Invalid = 0 };


    /////////////////////////////////////////////////////////
    //                                                     //
    //                    DRAW PARAMETERS                  //
    //                                                     //
    /////////////////////////////////////////////////////////

    struct SpriteParams {
        Vec2 position = {};             // Virtual coordinates of the pivot
        std::optional<Rect> source = {}; // Texture region (sprite-sheet frame), whole texture if empty
        Vec2 origin = {};               // Normalized pivot: {0, 0} top-left, {0.5, 0.5} center
        float rotation = 0.0F;          // Degrees, clockwise, around the pivot
        Vec2 scale = {1.0F, 1.0F};      // Must be positive: mirror with flipX / flipY
        Color tint = Colors::WHITE;
        bool flipX = false;
        bool flipY = false;
    };


    /////////////////////////////////////////////////////////
    //                                                     //
    //                        AUDIO                        //
    //                                                     //
    /////////////////////////////////////////////////////////

    enum class Channel : std::uint8_t {
        Master,
        Music,
        Effects,
    };
}

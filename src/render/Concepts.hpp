/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Render concepts
*/

#pragma once

#include "InputTypes.hpp"
#include "Types.hpp"
#include <wrecks/common.hpp>
#include <concepts>

namespace RType::Render {

    /**
     * What game code (e.g. ECS systems) may use to draw.
     * Satisfied by `RType::Render::Renderer`; tests can provide
     * a fake implementation.
     */
    template <typename T>
    concept RendererType =
        requires (const T& renderer, TextureId texture, FontId font, const SpriteParams& params,
                  Rect rect, Vec2D point, Color color, float value, Wrecks::cstring_view text)
        {
            { renderer.drawSprite(texture, params) } -> std::same_as<void>;
            { renderer.drawScrolling(texture, rect, point, value, color) } -> std::same_as<void>;
            { renderer.drawRect(rect, color) } -> std::same_as<void>;
            { renderer.drawRectLines(rect, value, color) } -> std::same_as<void>;
            { renderer.drawCircle(point, value, color) } -> std::same_as<void>;
            { renderer.drawLine(point, point, value, color) } -> std::same_as<void>;
            { renderer.drawText(text, point, value, color, font) } -> std::same_as<void>;
            { renderer.measureText(text, value, font) } -> std::same_as<Vec2D>;
        };

    /**
     * Read-only input queries.
     * Satisfied by `RType::Render::Input`; used by `ActionMap`.
     */
    template <typename T>
    concept InputType =
        requires (const T& input, Key key, MouseButton mouse, int gamepad,
                  GamepadButton button, GamepadAxis axis)
        {
            { input.isDown(key) } -> std::same_as<bool>;
            { input.isPressed(key) } -> std::same_as<bool>;
            { input.isReleased(key) } -> std::same_as<bool>;
            { input.isDown(mouse) } -> std::same_as<bool>;
            { input.isPressed(mouse) } -> std::same_as<bool>;
            { input.isReleased(mouse) } -> std::same_as<bool>;
            { input.isDown(gamepad, button) } -> std::same_as<bool>;
            { input.isPressed(gamepad, button) } -> std::same_as<bool>;
            { input.isReleased(gamepad, button) } -> std::same_as<bool>;
            { input.axis(gamepad, axis) } -> std::same_as<float>;
        };

    /**
     * What game code may use to play audio.
     * Satisfied by `RType::Render::Audio`.
     */
    template <typename T>
    concept AudioType =
        requires (T& audio, const T& cnstAudio, SoundId sound, MusicId music, Channel channel, float volume, bool loop)
        {
            { audio.play(sound) } -> std::same_as<void>;
            { audio.playMusic(music, loop) } -> std::same_as<void>;
            { audio.stopMusic() } -> std::same_as<void>;
            { audio.setVolume(channel, volume) } -> std::same_as<void>;
            { cnstAudio.volume(channel) } -> std::same_as<float>;
        };
}

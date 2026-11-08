/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Popup component
*/

#pragma once

#include <rtype/render.hpp>
#include <string>


namespace RType::Components {
    /**
     * Short text rising from where something happened, then
     * fading ("+1 LIFE" over a ship that took a bonus): the
     * visual cue of a sound. An entity with a Transform and a
     * Lifetime of `duration`: Systems::Expire removes it.
     */
    struct Popup {
        std::string text;
        Render::Color color = Render::Colors::WHITE;
        float duration = 0.0F;  // seconds
    };

    inline constexpr float POPUP_RISE = 30.0F;  // screen pixels per second

    // Opacity from 1 to 0 over the second half of its life
    constexpr float popupOpacity(float remaining, float duration) noexcept
    {
        if (duration <= 0.0F || remaining <= 0.0F)
            return 0.0F;
        return remaining >= duration / 2.0F ? 1.0F : remaining / (duration / 2.0F);
    }

    // Screen pixels risen since it appeared
    constexpr float popupRise(float remaining, float duration) noexcept
    {
        float shown = duration - remaining;

        return shown > 0.0F ? shown * POPUP_RISE : 0.0F;
    }
}

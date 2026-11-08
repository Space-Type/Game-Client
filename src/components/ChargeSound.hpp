/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** ChargeSound component
*/

#pragma once

#include "Charge.hpp"
#include <rtype/render.hpp>


namespace RType::Components {
    // The charge sound of the local ship, and whether it is playing (see Systems::PlayChargeSound)
    struct ChargeSound {
        Render::SoundId sound = Render::SoundId::Invalid;
        bool playing = false;
    };

    enum class SoundChange : std::uint8_t {
        None,
        Play,
        Stop,
    };

    // The sound plays while the trigger is held (not for a quick tap) and stops when it is released
    constexpr SoundChange chargeSoundChange(float chargeSeconds, bool playing) noexcept
    {
        bool charging = chargeSeconds >= CHARGE_SOUND_DELAY;

        if (charging && !playing)
            return SoundChange::Play;
        if (!charging && playing)
            return SoundChange::Stop;
        return SoundChange::None;
    }
}

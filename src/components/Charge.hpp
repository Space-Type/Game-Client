/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** charged shot state shown by the client
*/

#pragma once

#include <cstdint>


namespace RType::Components {
    // Same rules as the server (Server: Rules::CHARGE_LEVEL_TIME, CHARGE_LEVELS)
    inline constexpr float CHARGE_LEVEL_TIME = 0.4F;  // seconds per charge level
    inline constexpr std::uint8_t CHARGE_LEVELS = 5;
    inline constexpr float CHARGE_SOUND_DELAY = 0.2F;  // a quick tap does not play the charge sound

    /**
     * How long the local player has held the trigger: its
     * charge shows at once, without waiting for the server
     * (PlayerStats::charge gives it for the other players).
     */
    struct LocalCharge {
        float seconds = 0.0F;
    };

    // Charge level (0 to CHARGE_LEVELS) after holding the trigger for `seconds`
    constexpr std::uint8_t chargeLevel(float seconds) noexcept
    {
        auto level = static_cast<int>(seconds / CHARGE_LEVEL_TIME);

        return static_cast<std::uint8_t>(level < 0 ? 0 : (level > CHARGE_LEVELS ? CHARGE_LEVELS : level));
    }
}

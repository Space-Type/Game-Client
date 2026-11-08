/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Announcement component
*/

#pragma once

#include "Lifetime.hpp"
#include <string>


namespace RType::Components {
    /**
     * Big text over the game: "LEVEL n" when a stage starts,
     * "STAGE CLEAR" (with the scores) when it is won. An entity
     * with a Lifetime of `duration`: Systems::Expire removes it.
     */
    struct Announcement {
        std::string text;
        float duration = 0.0F;  // seconds
        bool scores = false;    // also shows every player's score
    };

    // Seconds the announcement has been shown
    constexpr float shownFor(const Announcement& announcement, const Lifetime& lifetime) noexcept
    {
        return announcement.duration - lifetime.remaining;
    }
}

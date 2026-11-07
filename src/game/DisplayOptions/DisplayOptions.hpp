/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** display options
*/

#pragma once


namespace RType::Game {
    /**
     * Accessibility options of the game display,
     * owned by the Game and read every frame by the
     * render systems: a change applies at once.
     */
    struct DisplayOptions {
        bool highContrast = false;  // outlines every entity, coloured by type
    };
}

/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Music component
*/

#pragma once

#include <string>


namespace RType::Components {
    /**
     * The soundtrack: the music Systems::PlayMusic plays, as a
     * path under assets/ ("musics/title.ogg"). Empty: silence.
     * Changing it switches the music.
     */
    struct Music {
        std::string track;
    };
}

/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** SceneId enum
*/

#pragma once


namespace RType::Scenes {
    // Scene order of Manager. Quit has no scene: it ends the game
    enum class SceneId {
        Menu,
        Connecting,
        InGame,
        Paused,
        GameOver,
        Quit,
    };
}

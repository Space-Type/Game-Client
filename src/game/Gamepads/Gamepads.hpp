/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** gamepad mappings and choice
*/

#pragma once

#include <rtype/render.hpp>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>


namespace RType::Game {
    // A gamepad the game can read (connected and mapped)
    struct ConnectedGamepad {
        int index;
        std::string name;
    };

    std::vector<ConnectedGamepad> connectedGamepads(const Render::Input& input);

    // Joysticks connected without a mapping: the player can add one (see playerMappingsPath)
    std::vector<std::string> unknownControllers(const Render::Input& input);

    // The gamepad named `preferred` if it is connected, else the first one; -1 if there is none
    int chooseGamepad(const std::vector<ConnectedGamepad>& gamepads, std::string_view preferred) noexcept;

    // The settings cycle through "" (the first gamepad found), then each connected gamepad by name
    std::string nextGamepadChoice(const std::vector<ConnectedGamepad>& gamepads, std::string_view current);

    // `gamecontrollerdb.txt` next to settings.cfg, where a player adds the mappings of their own controllers
    std::optional<std::filesystem::path> playerMappingsPath();

    /**
     * Loads the gamepad mappings, each source overriding the
     * previous one for the same controller: the database
     * shipped next to the client (vcpkg port
     * sdl-gamecontrollerdb), then the player's file, then the
     * SDL_GAMECONTROLLERCONFIG environment variable (as with
     * SDL and Steam). Needs the window to be open.
     */
    void loadGamepadMappings(Render::Input& input);
}

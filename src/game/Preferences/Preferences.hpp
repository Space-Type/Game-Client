/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** player preferences
*/

#pragma once

#include "../Controls/Controls.hpp"
#include "../DisplayOptions/DisplayOptions.hpp"
#include <rtype/render.hpp>
#include <cstddef>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>


namespace RType::Game {
    /**
     * What the settings screen changes, kept between runs
     * in a `key=value` text file:
     *
     *     volume.master=80
     *     fullscreen=false
 *     accessibility.high_contrast=false
     *     bind.shoot.keyboard=Space
     *     bind.shoot.gamepad=A
     *
     * Inputs are stored by name, never by enum value.
     */
    struct Preferences {
        static constexpr int VOLUME_MAX = 100;

        // The biggest file possible could be ~436 bytes with the longer names possible. 4KiB leaves room for more settings later.
        static constexpr std::size_t FILE_SIZE_MAX = 4 * 1024;

        int master = VOLUME_MAX;    // volumes in percent
        int music = VOLUME_MAX;
        int effects = VOLUME_MAX;
        bool fullscreen = false;
        bool highContrast = false;
        Controls controls = defaultControls();
    };

    Preferences currentPreferences(const Render::Audio& audio, const Render::Window& window, const Controls& controls,
                                   const DisplayOptions& display);

    // Keeps the gamepad selected in `controls`
    void applyPreferences(const Preferences& preferences, Render::Audio& audio, Render::Window& window, Controls& controls,
                          DisplayOptions& display);

    std::string toText(const Preferences& preferences);

    /**
     * Reads the lines it knows on top of `defaults`. Unknown
     * keys, bad values and out-of-range volumes are skipped:
     * a broken file never stops the game.
     */
    Preferences fromText(std::string_view text, Preferences defaults);

    /**
     * `%APPDATA%\r-type\settings.cfg` on Windows,
     * `$XDG_CONFIG_HOME/r-type/settings.cfg` (or
     * `~/.config/...`) elsewhere. nullopt if none is set.
     */
    std::optional<std::filesystem::path> preferencesPath();

    // Defaults if the file is missing, unreadable or bigger than FILE_SIZE_MAX
    Preferences loadPreferences(Preferences defaults);

    // Creates the folder if needed; false (and a message on stderr) on failure
    bool savePreferences(const Preferences& preferences);
}

/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** scene Context
*/

#pragma once

#include <rtype/render.hpp>
#include <rtype/ui.hpp>
#include "../game/ConnectionInfo/ConnectionInfo.hpp"
#include "../game/Controls/Controls.hpp"
#include "../game/DisplayOptions/DisplayOptions.hpp"
#include "../game/Session/Session.hpp"
#include "SceneId.hpp"
#include <wrecks/common.hpp>
#include <cstdint>
#include <memory>
#include <string>


namespace RType::Scenes {
    // Everything a scene may use, owned by the Game
    struct Context {
        Render::Window& window;
        const Render::Renderer& renderer;
        Render::Input& input;
        Render::Audio& audio;
        Render::Resources& resources;
        Game::Controls& controls;                 // changed by Settings
        const UI::MenuControls& menuControls;
        Game::DisplayOptions& displayOptions;     // changed by Settings
        std::unique_ptr<Game::Session>& session;  // null outside of a game
        std::string message;                      // shown by GameOver and Connect
        Game::Outcome outcome;                    // how the last game ended, for GameOver
        std::uint32_t score;                      // its score, for a victory or a defeat
        Game::ConnectionInfo connection;          // prefills Connect
        SceneId settingsReturn;                   // where Settings goes back to
        Render::SoundId clickSound;               // played on every Form event
    };
}

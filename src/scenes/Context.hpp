/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** scene Context
*/

#pragma once

#include <rtype/render.hpp>
#include "../game/Controls/Controls.hpp"
#include "../game/Session/Session.hpp"
#include <wrecks/common.hpp>
#include <memory>
#include <string>


namespace RType::Scenes {
    // Everything a scene may use, owned by the Game
    struct Context {
        Render::Window& window;
        const Render::Renderer& renderer;
        const Render::Input& input;
        Render::Resources& resources;
        const Game::Controls& controls;
        std::unique_ptr<Game::Session>& session;  // null outside of a game
        Wrecks::DeltaTime fixedUpdateRate;
        std::string message;                      // shown by GameOver
    };
}

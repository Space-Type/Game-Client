/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Paused scene
*/

#pragma once

#include "../Context.hpp"
#include "../SceneId.hpp"
#include <wrecks/common.hpp>
#include <optional>


namespace RType::Scenes {
    // Overlay over the session (still running, controls disabled): resume or disconnect
    class Paused {
        public:
            std::optional<SceneId> update(Context& ctx, Wrecks::DeltaTime dt);
            void render(Context& ctx) const;
    };
}

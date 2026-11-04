/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** GameOver scene
*/

#pragma once

#include "../Context.hpp"
#include "../SceneId.hpp"
#include <wrecks/common.hpp>
#include <optional>


namespace RType::Scenes {
    // End of a game or failed connection: shows Context::message
    class GameOver {
        public:
            std::optional<SceneId> update(Context& ctx, Wrecks::DeltaTime dt);
            void render(Context& ctx) const;
    };
}

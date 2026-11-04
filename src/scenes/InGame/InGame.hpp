/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** InGame scene
*/

#pragma once

#include "../Context.hpp"
#include "../SceneId.hpp"
#include <wrecks/common.hpp>
#include <optional>


namespace RType::Scenes {
    // Playing: the session draws itself, this scene only handles pausing
    class InGame {
        public:
            std::optional<SceneId> update(Context& ctx, Wrecks::DeltaTime dt);
            void render(Context& ctx) const;
    };
}

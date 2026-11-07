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
    // Playing: the session draws itself, this scene handles pausing and draws the HUD
    class InGame {
        float m_Seconds = 0.0F;  // drives the HUD blinking

        public:
            std::optional<SceneId> update(Context& ctx, Wrecks::DeltaTime dt);
            void render(Context& ctx) const;
    };
}

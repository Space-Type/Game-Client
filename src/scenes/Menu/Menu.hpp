/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Menu scene
*/

#pragma once

#include "../Context.hpp"
#include "../SceneId.hpp"
#include <wrecks/common.hpp>
#include <optional>


namespace RType::Scenes {
    // Title screen: play or quit
    class Menu {
        public:
            std::optional<SceneId> update(Context& ctx, Wrecks::DeltaTime dt);
            void render(Context& ctx) const;
    };
}

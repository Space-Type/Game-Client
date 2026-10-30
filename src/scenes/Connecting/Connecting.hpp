/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Connecting scene
*/

#pragma once

#include "../Context.hpp"
#include "../SceneId.hpp"
#include <wrecks/common.hpp>
#include <optional>


namespace RType::Scenes {
    // Opens the session, shows a message while the connection blocks
    class Connecting {
        bool m_Started;

        public:
            Connecting() noexcept;

            std::optional<SceneId> update(Context& ctx, Wrecks::DeltaTime dt);
            void render(Context& ctx) const;
    };
}

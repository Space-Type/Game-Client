/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Connect scene
*/

#pragma once

#include "../Context.hpp"
#include "../SceneId.hpp"
#include <rtype/ui.hpp>
#include <wrecks/common.hpp>
#include <optional>
#include <string>


namespace RType::Scenes {
    // First screen: where to connect and as whom, or quit
    class Connect {
        std::optional<UI::Form> m_Form;  // built on the first update: scenes have no constructor argument
        std::string m_Error;             // what validation found wrong

        public:
            Connect() noexcept = default;

            std::optional<SceneId> update(Context& ctx, Wrecks::DeltaTime dt);
            void render(Context& ctx) const;
    };
}

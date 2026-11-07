/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Settings scene
*/

#pragma once

#include "../Context.hpp"
#include "../SceneId.hpp"
#include <wrecks/common.hpp>
#include <cstddef>
#include <optional>


namespace RType::Scenes {
    /**
     * The list of settings sections, then the page of
     * the chosen one. Changes apply right away. Back
     * leaves a page for the list, then the list for
     * Context::settingsReturn.
     */
    class Settings {
        public:
            // A control slot waiting for the player's next input
            struct Capture {
                Game::Action action;
                Game::Slot slot;
            };

        private:
            std::optional<std::size_t> m_Page;  // section shown, none for the list
            std::optional<UI::Form> m_Form;     // built on the first update: scenes have no constructor argument
            std::optional<Capture> m_Capture;

        public:
            Settings() noexcept = default;

            std::optional<SceneId> update(Context& ctx, Wrecks::DeltaTime dt);
            void render(Context& ctx) const;

        private:
            void open(const Context& ctx, std::optional<std::size_t> page);
            void updateCapture(Context& ctx);
    };
}

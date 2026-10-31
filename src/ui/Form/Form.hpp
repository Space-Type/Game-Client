/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Form class
*/

#pragma once

#include "../Button/Button.hpp"
#include "../FrameInput/FrameInput.hpp"
#include "../TextField/TextField.hpp"
#include <rtype/render.hpp>
#include <cstddef>
#include <optional>
#include <variant>
#include <vector>


namespace RType::UI {
    using Widget = std::variant<Button, TextField>;

    /**
     * Widgets and which one has the focus.
     *
     * Up and Down wrap around. Confirm moves from a text
     * field to the next widget, and activates a button.
     * The mouse only takes the focus when it moves (a still
     * mouse never steals it from the keyboard), a click
     * focuses a text field and activates a button.
     */
    class Form {
        std::vector<Widget> m_Widgets;
        std::size_t m_Focus;
        std::optional<Render::Vec2D> m_LastMouse;

        public:
            explicit Form(std::vector<Widget> widgets);

            // Returns the index of the button activated this frame, if any
            std::optional<std::size_t> update(const FrameInput& input);

            void draw(const Render::Renderer& renderer) const;

            [[ nodiscard ]] inline std::size_t focus() const noexcept { return m_Focus; }
            [[ nodiscard ]] inline const std::vector<Widget>& widgets() const noexcept { return m_Widgets; }

        private:
            void moveFocus(bool forward) noexcept;
            [[ nodiscard ]] std::optional<std::size_t> widgetAt(Render::Vec2D point) const;
            [[ nodiscard ]] bool isButton(std::size_t index) const noexcept;
    };
}

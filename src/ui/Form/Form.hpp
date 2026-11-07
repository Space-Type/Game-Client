/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Form class
*/

#pragma once

#include "../Button/Button.hpp"
#include "../Checkbox/Checkbox.hpp"
#include "../FrameInput/FrameInput.hpp"
#include "../Slider/Slider.hpp"
#include "../TextField/TextField.hpp"
#include <rtype/render.hpp>
#include <cstddef>
#include <optional>
#include <variant>
#include <vector>


namespace RType::UI {
    using Widget = std::variant<Button, TextField, Slider, Checkbox>;

    // What happened to a widget of a Form during one frame
    struct FormEvent {
        enum class Kind {
            Activated,  // a button was pressed
            Changed,    // the value of a widget changed
        };

        Kind kind;
        std::size_t index;

        bool operator==(const FormEvent&) const = default;
    };

    /**
     * Widgets and which one has the focus.
     *
     * Up and Down wrap around. Confirm moves from a text
     * field or a slider to the next widget, activates a
     * button and toggles a checkbox. Left and Right only
     * move a slider or set a checkbox off and on.
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

            // Returns what happened this frame, if anything
            std::optional<FormEvent> update(const FrameInput& input);

            void draw(const Render::Renderer& renderer) const;

            [[ nodiscard ]] inline std::size_t focus() const noexcept { return m_Focus; }
            // Ignored if out of range
            inline void setFocus(std::size_t index) noexcept { if (index < m_Widgets.size()) m_Focus = index; }
            [[ nodiscard ]] inline const std::vector<Widget>& widgets() const noexcept { return m_Widgets; }

        private:
            void moveFocus(bool forward) noexcept;
            [[ nodiscard ]] std::optional<std::size_t> widgetAt(Render::Vec2D point) const;
            [[ nodiscard ]] bool isButton(std::size_t index) const noexcept;
    };
}

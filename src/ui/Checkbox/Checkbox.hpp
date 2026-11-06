/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Checkbox class
*/

#pragma once

#include "../FrameInput/FrameInput.hpp"
#include <rtype/render.hpp>
#include <string>


namespace RType::UI {
    /**
     * A labelled on/off value.
     *
     * Confirm and a click on its box toggle it. Left sets it
     * off and Right on, like a slider next to it in a list.
     */
    class Checkbox {
        std::string m_Label;
        bool m_Checked;
        Render::Rect m_Bounds;  // the whole row: the square, then the label

        public:
            Checkbox(std::string label, bool checked, Render::Rect bounds);

            [[ nodiscard ]] inline const std::string& label() const noexcept { return m_Label; }
            [[ nodiscard ]] inline bool checked() const noexcept { return m_Checked; }
            [[ nodiscard ]] inline Render::Rect bounds() const noexcept { return m_Bounds; }

            // Applies the actions and the click of this frame. Returns true if it changed
            bool update(const FrameInput& input) noexcept;

            void draw(const Render::Renderer& renderer, bool focused) const;
    };
}

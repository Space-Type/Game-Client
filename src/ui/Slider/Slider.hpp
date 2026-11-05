/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Slider class
*/

#pragma once

#include "../FrameInput/FrameInput.hpp"
#include <rtype/render.hpp>
#include <string>


namespace RType::UI {
    /**
     * A labelled integer value between a minimum and a
     * maximum, moved by steps.
     *
     * Left and Right move it by one step. The mouse drags
     * it: a click on the box starts the drag, which goes on
     * while the button is held, even outside the box.
     * The value is always kept in range and on a step.
     */
    class Slider {
        std::string m_Label;
        int m_Value;
        int m_Min;
        int m_Max;
        int m_Step;
        std::string m_Unit;
        Render::Rect m_Bounds;  // the box: the label is drawn on its left
        bool m_Dragging;

        public:
            Slider(std::string label, int value, int min, int max, int step,
                   Render::Rect bounds, std::string unit = "");

            [[ nodiscard ]] inline const std::string& label() const noexcept { return m_Label; }
            [[ nodiscard ]] inline int value() const noexcept { return m_Value; }
            [[ nodiscard ]] inline Render::Rect bounds() const noexcept { return m_Bounds; }
            [[ nodiscard ]] inline bool dragging() const noexcept { return m_Dragging; }

            // Where the value is drawn and dragged, inside the box
            [[ nodiscard ]] Render::Rect track() const noexcept;

            // Applies the arrows and the mouse of this frame. Returns true if the value changed
            bool update(const FrameInput& input) noexcept;

            void draw(const Render::Renderer& renderer, bool focused) const;

        private:
            [[ nodiscard ]] int clamp(int value) const noexcept;
            [[ nodiscard ]] int valueAt(float x) const noexcept;
    };
}
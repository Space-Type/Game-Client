/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Button class
*/

#pragma once

#include <rtype/render.hpp>
#include <string>


namespace RType::UI {
    // A label and its clickable area. Activated through its Form
    class Button {
        std::string m_Label;
        Render::Rect m_Bounds;

        public:
            Button(std::string label, Render::Rect bounds);

            [[ nodiscard ]] inline const std::string& label() const noexcept { return m_Label; }
            [[ nodiscard ]] inline Render::Rect bounds() const noexcept { return m_Bounds; }

            void draw(const Render::Renderer& renderer, bool focused) const;
    };
}

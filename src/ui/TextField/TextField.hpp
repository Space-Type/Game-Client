/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** TextField class
*/

#pragma once

#include "../FrameInput/FrameInput.hpp"
#include <rtype/render.hpp>
#include <cstddef>
#include <string>


namespace RType::UI {
    /**
     * A labelled line of text, edited at its end only.
     *
     * Only printable ASCII is accepted (all the default
     * font can draw), further restricted by the filter if
     * any. An initial value is kept as is, even if it does
     * not follow these rules: it can always be erased.
     */
    class TextField {
        public:
            using Filter = bool (*)(char);

        private:
            std::string m_Label;
            std::string m_Value;
            std::size_t m_MaxLength;
            Render::Rect m_Bounds;  // the box: the label is drawn on its left
            Filter m_Filter;

        public:
            TextField(std::string label, std::string value, std::size_t maxLength,
                      Render::Rect bounds, Filter filter = nullptr);

            [[ nodiscard ]] inline const std::string& label() const noexcept { return m_Label; }
            [[ nodiscard ]] inline const std::string& value() const noexcept { return m_Value; }
            [[ nodiscard ]] inline Render::Rect bounds() const noexcept { return m_Bounds; }

            // Applies the text typed and Backspace pressed this frame
            void update(const FrameInput& input);

            void draw(const Render::Renderer& renderer, bool focused) const;

        private:
            void type(char32_t character);
    };

    // TextField filter for numbers
    bool isDigit(char character) noexcept;
}

/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Button class
*/

#include "Button.hpp"
#include "../Theme.hpp"
#include <utility>


RType::UI::Button::Button(std::string label, Render::Rect bounds)
    : m_Label(std::move(label))
    , m_Bounds(bounds)
{

}

void RType::UI::Button::draw(const Render::Renderer& renderer, bool focused) const
{
    Wrecks::cstring_view label(m_Label);
    Render::Vec2D size = renderer.measureText(label, Theme::TEXT_SIZE);
    float textY = m_Bounds.y + (m_Bounds.height - Theme::TEXT_SIZE) / 2.0F;

    renderer.drawRect(m_Bounds, Theme::BACKGROUND);
    renderer.drawRectLines(m_Bounds, focused ? Theme::FOCUS_THICKNESS : Theme::BORDER_THICKNESS,
                           focused ? Theme::FOCUS_COLOR : Theme::BORDER_COLOR);
    renderer.drawText(label, { .x = m_Bounds.x + (m_Bounds.width - size.x) / 2.0F, .y = textY },
                      Theme::TEXT_SIZE, Theme::FOREGROUND);

    if (focused)
        renderer.drawText(Theme::FOCUS_MARKER, { .x = m_Bounds.x - Theme::MARKER_GAP, .y = textY },
                          Theme::TEXT_SIZE, Theme::FOCUS_COLOR);
}

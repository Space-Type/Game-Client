/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Checkbox class
*/

#include "Checkbox.hpp"
#include "../Theme.hpp"
#include <utility>


RType::UI::Checkbox::Checkbox(std::string label, bool checked, Render::Rect bounds)
    : m_Label(std::move(label))
    , m_Checked(checked)
    , m_Bounds(bounds)
{

}

bool RType::UI::Checkbox::update(const FrameInput& input) noexcept
{
    bool previous = m_Checked;
    bool onBox = input.mouse.x >= m_Bounds.x && input.mouse.x < m_Bounds.x + m_Bounds.width
              && input.mouse.y >= m_Bounds.y && input.mouse.y < m_Bounds.y + m_Bounds.height;

    if (input.isPressed(MenuAction::Confirm) || (input.click && onBox))
        m_Checked = !m_Checked;
    if (input.isPressed(MenuAction::Left))
        m_Checked = false;
    if (input.isPressed(MenuAction::Right))
        m_Checked = true;

    return m_Checked != previous;
}

/**
 * The state shows as a shape, a cross in
 * the square, not only as a colour.
 */
void RType::UI::Checkbox::draw(const Render::Renderer& renderer, bool focused) const
{
    float textY = m_Bounds.y + (m_Bounds.height - Theme::TEXT_SIZE) / 2.0F;
    Render::Rect square = {
        .x = m_Bounds.x + Theme::PADDING,
        .y = m_Bounds.y + (m_Bounds.height - Theme::CHECK_SIZE) / 2.0F,
        .width = Theme::CHECK_SIZE,
        .height = Theme::CHECK_SIZE,
    };

    renderer.drawRect(m_Bounds, Theme::BACKGROUND);
    renderer.drawRectLines(m_Bounds, focused ? Theme::FOCUS_THICKNESS : Theme::BORDER_THICKNESS,
                           focused ? Theme::FOCUS_COLOR : Theme::BORDER_COLOR);
    renderer.drawRectLines(square, Theme::BORDER_THICKNESS, Theme::FOREGROUND);

    if (m_Checked) {
        float inset = Theme::CHECK_INSET;
        float left = square.x + inset;
        float right = square.x + square.width - inset;
        float top = square.y + inset;
        float bottom = square.y + square.height - inset;

        renderer.drawLine({ .x = left, .y = top }, { .x = right, .y = bottom }, Theme::CHECK_THICKNESS, Theme::FOREGROUND);
        renderer.drawLine({ .x = left, .y = bottom }, { .x = right, .y = top }, Theme::CHECK_THICKNESS, Theme::FOREGROUND);
    }

    renderer.drawText(Wrecks::cstring_view(m_Label), { .x = square.x + square.width + Theme::PADDING, .y = textY },
                      Theme::TEXT_SIZE, Theme::FOREGROUND);

    if (focused)
        renderer.drawText(Theme::FOCUS_MARKER, { .x = m_Bounds.x - Theme::MARKER_GAP, .y = textY },
                          Theme::TEXT_SIZE, Theme::FOCUS_COLOR);
}

/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Slider class
*/

#include "Slider.hpp"
#include "../Theme.hpp"
#include <algorithm>
#include <cmath>
#include <utility>


/**
 * A step of 0 or less is taken as 1, and a maximum
 * under the minimum as the minimum: the range is
 * never empty.
 */
RType::UI::Slider::Slider(std::string label, int value, int min, int max, int step,
                          Render::Rect bounds, std::string unit)
    : m_Label(std::move(label))
    , m_Value(0)
    , m_Min(min)
    , m_Max(std::max(min, max))
    , m_Step(std::max(step, 1))
    , m_Unit(std::move(unit))
    , m_Bounds(bounds)
    , m_Dragging(false)
{
    m_Value = clamp(value);
}

auto RType::UI::Slider::track() const noexcept -> Render::Rect
{
    return {
        .x = m_Bounds.x + Theme::PADDING,
        .y = m_Bounds.y + (m_Bounds.height - Theme::TRACK_HEIGHT) / 2.0F,
        .width = std::max(m_Bounds.width - 2.0F * Theme::PADDING - Theme::VALUE_WIDTH, 0.0F),
        .height = Theme::TRACK_HEIGHT,
    };
}

bool RType::UI::Slider::update(const FrameInput& input) noexcept
{
    int previous = m_Value;
    const Render::Rect& box = m_Bounds;
    bool onBox = input.mouse.x >= box.x && input.mouse.x < box.x + box.width
              && input.mouse.y >= box.y && input.mouse.y < box.y + box.height;

    if (input.isPressed(MenuAction::Left))
        m_Value = clamp(m_Value - m_Step);
    if (input.isPressed(MenuAction::Right))
        m_Value = clamp(m_Value + m_Step);

    if (input.click && onBox)
        m_Dragging = true;
    if (!input.mouseDown && !input.click)
        m_Dragging = false;
    if (m_Dragging)
        m_Value = valueAt(input.mouse.x);

    return m_Value != previous;
}

// Rounds to the nearest step from the minimum, then keeps it in range
int RType::UI::Slider::clamp(int value) const noexcept
{
    long steps = std::lround(static_cast<double>(value - m_Min) / m_Step);

    return std::clamp(static_cast<int>(m_Min + steps * m_Step), m_Min, m_Max);
}

int RType::UI::Slider::valueAt(float x) const noexcept
{
    Render::Rect bar = track();

    if (bar.width <= 0.0F)
        return m_Value;

    double ratio = std::clamp(static_cast<double>((x - bar.x) / bar.width), 0.0, 1.0);

    return clamp(m_Min + static_cast<int>(std::lround(ratio * (m_Max - m_Min))));
}

/**
 * The value is also written as text: the
 * fill is not the only way to read it.
 */
void RType::UI::Slider::draw(const Render::Renderer& renderer, bool focused) const
{
    float textY = m_Bounds.y + (m_Bounds.height - Theme::TEXT_SIZE) / 2.0F;
    float labelX = m_Bounds.x - Theme::LABEL_WIDTH;
    Render::Rect bar = track();
    Render::Rect fill = bar;
    std::string shown = std::to_string(m_Value) + m_Unit;

    if (m_Max > m_Min)
        fill.width = bar.width * static_cast<float>(m_Value - m_Min) / static_cast<float>(m_Max - m_Min);

    renderer.drawText(Wrecks::cstring_view(m_Label), { .x = labelX, .y = textY }, Theme::TEXT_SIZE, Theme::FOREGROUND);
    renderer.drawRect(m_Bounds, Theme::BACKGROUND);
    renderer.drawRectLines(m_Bounds, focused ? Theme::FOCUS_THICKNESS : Theme::BORDER_THICKNESS,
                           focused ? Theme::FOCUS_COLOR : Theme::BORDER_COLOR);
    renderer.drawRect(bar, Theme::TRACK_COLOR);
    renderer.drawRect(fill, Theme::FILL_COLOR);
    renderer.drawText(Wrecks::cstring_view(shown), { .x = bar.x + bar.width + Theme::PADDING, .y = textY },
                      Theme::TEXT_SIZE, Theme::FOREGROUND);

    if (focused)
        renderer.drawText(Theme::FOCUS_MARKER, { .x = labelX - Theme::MARKER_GAP, .y = textY },
                          Theme::TEXT_SIZE, Theme::FOCUS_COLOR);
}
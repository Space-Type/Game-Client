/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Form class
*/

#include "Form.hpp"
#include <utility>


RType::UI::Form::Form(std::vector<Widget> widgets)
    : m_Widgets(std::move(widgets))
    , m_Focus(0)
    , m_LastMouse()
{

}

/**
 * Text goes first, to the widget focused when it was
 * typed: it must not follow a focus change of the same frame.
 */
auto RType::UI::Form::update(const FrameInput& input) -> std::optional<std::size_t>
{
    if (m_Widgets.empty())
        return std::nullopt;

    if (auto* field = std::get_if<TextField>(&m_Widgets[m_Focus]))
        field->update(input);

    std::optional<std::size_t> hovered = widgetAt(input.mouse);
    bool mouseMoved = m_LastMouse && (m_LastMouse->x != input.mouse.x || m_LastMouse->y != input.mouse.y);

    m_LastMouse = input.mouse;

    if (hovered && (mouseMoved || input.click))
        m_Focus = *hovered;
    if (hovered && input.click && isButton(*hovered))
        return hovered;

    if (input.isPressed(MenuAction::Up))
        moveFocus(false);
    if (input.isPressed(MenuAction::Down))
        moveFocus(true);

    if (input.isPressed(MenuAction::Confirm)) {
        if (isButton(m_Focus))
            return m_Focus;
        moveFocus(true);
    }
    return std::nullopt;
}

void RType::UI::Form::draw(const Render::Renderer& renderer) const
{
    for (std::size_t i = 0; i < m_Widgets.size(); ++i)
        std::visit([&](const auto& widget){ widget.draw(renderer, i == m_Focus); }, m_Widgets[i]);
}

void RType::UI::Form::moveFocus(bool forward) noexcept
{
    m_Focus = (m_Focus + (forward ? 1 : m_Widgets.size() - 1)) % m_Widgets.size();
}

auto RType::UI::Form::widgetAt(Render::Vec2D point) const -> std::optional<std::size_t>
{
    for (std::size_t i = 0; i < m_Widgets.size(); ++i) {
        Render::Rect rect = std::visit([](const auto& widget){ return widget.bounds(); }, m_Widgets[i]);

        if (point.x >= rect.x && point.x < rect.x + rect.width && point.y >= rect.y && point.y < rect.y + rect.height)
            return i;
    }
    return std::nullopt;
}

bool RType::UI::Form::isButton(std::size_t index) const noexcept
{
    return std::holds_alternative<Button>(m_Widgets[index]);
}

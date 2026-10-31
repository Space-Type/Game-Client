/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** TextField class
*/

#include "TextField.hpp"
#include "../Theme.hpp"
#include <utility>


namespace {
    constexpr char32_t FIRST_PRINTABLE = U' ';
    constexpr char32_t LAST_PRINTABLE = U'~';
}


RType::UI::TextField::TextField(std::string label, std::string value, std::size_t maxLength,
                                Render::Rect bounds, Filter filter)
    : m_Label(std::move(label))
    , m_Value(std::move(value))
    , m_MaxLength(maxLength)
    , m_Bounds(bounds)
    , m_Filter(filter)
{

}

void RType::UI::TextField::update(const FrameInput& input)
{
    for (char32_t character : input.text)
        type(character);

    if (input.backspace && !m_Value.empty())
        m_Value.pop_back();
}

void RType::UI::TextField::type(char32_t character)
{
    if (character < FIRST_PRINTABLE || character > LAST_PRINTABLE || m_Value.size() >= m_MaxLength)
        return;

    char ascii = static_cast<char>(character);

    if (!m_Filter || m_Filter(ascii))
        m_Value.push_back(ascii);
}

/**
 * Only the end of a value too long
 * for the box is drawn: that is
 * where the text is edited.
 */
void RType::UI::TextField::draw(const Render::Renderer& renderer, bool focused) const
{
    float textY = m_Bounds.y + (m_Bounds.height - Theme::TEXT_SIZE) / 2.0F;
    float labelX = m_Bounds.x - Theme::LABEL_WIDTH;
    float maxWidth = m_Bounds.width - 2.0F * Theme::PADDING;
    std::string shown = focused ? m_Value + "_" : m_Value;

    while (!shown.empty() && renderer.measureText(Wrecks::cstring_view(shown), Theme::TEXT_SIZE).x > maxWidth)
        shown.erase(shown.begin());

    renderer.drawText(Wrecks::cstring_view(m_Label), { .x = labelX, .y = textY }, Theme::TEXT_SIZE, Theme::FOREGROUND);
    renderer.drawRect(m_Bounds, Theme::BACKGROUND);
    renderer.drawRectLines(m_Bounds, focused ? Theme::FOCUS_THICKNESS : Theme::BORDER_THICKNESS,
                           focused ? Theme::FOCUS_COLOR : Theme::BORDER_COLOR);
    renderer.drawText(Wrecks::cstring_view(shown), { .x = m_Bounds.x + Theme::PADDING, .y = textY },
                      Theme::TEXT_SIZE, Theme::FOREGROUND);

    if (focused)
        renderer.drawText(Theme::FOCUS_MARKER, { .x = labelX - Theme::MARKER_GAP, .y = textY },
                          Theme::TEXT_SIZE, Theme::FOCUS_COLOR);
}

bool RType::UI::isDigit(char character) noexcept
{
    return character >= '0' && character <= '9';
}

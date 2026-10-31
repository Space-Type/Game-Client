/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** FrameInput struct
*/

#include "FrameInput.hpp"


auto RType::UI::readFrameInput(Render::Input& input, const MenuControls& controls) -> FrameInput
{
    FrameInput frame;

    for (std::size_t i = 0; i < MENU_ACTION_COUNT; ++i) {
        auto action = static_cast<MenuAction>(i);

        if (controls.isPressed(action, input))
            frame.press(action);
    }

    frame.backspace = input.isPressed(Render::Key::Backspace);
    frame.mouse = input.mousePosition();
    frame.click = input.isPressed(Render::MouseButton::Left);

    for (auto typed = input.nextChar(); typed; typed = input.nextChar())
        frame.text.push_back(*typed);

    return frame;
}

/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** FrameInput struct
*/

#pragma once

#include "../MenuControls/MenuControls.hpp"
#include <rtype/render.hpp>
#include <bitset>
#include <string>


namespace RType::UI {
    /**
     * What a menu needs to know of the inputs of one
     * frame. Widgets only read this, so that their
     * logic can be tested without a window.
     */
    struct FrameInput {
        std::bitset<MENU_ACTION_COUNT> actions = {};  // pressed this frame
        bool backspace = false;
        Render::Vec2D mouse = {};
        bool click = false;
        std::u32string text = {};                     // typed this frame

        [[ nodiscard ]] inline bool isPressed(MenuAction action) const noexcept { return actions[static_cast<std::size_t>(action)]; }
        inline void press(MenuAction action) noexcept { actions[static_cast<std::size_t>(action)] = true; }
    };

    // Takes a non-const Input: empties its queue of typed characters
    FrameInput readFrameInput(Render::Input& input, const MenuControls& controls);
}

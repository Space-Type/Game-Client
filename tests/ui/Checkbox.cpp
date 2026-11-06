/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** Checkbox class
*/

#include <gtest/gtest.h>
#include <rtype/ui.hpp>

namespace {

    using RType::UI::Checkbox;
    using RType::UI::FrameInput;
    using RType::UI::MenuAction;

    constexpr RType::Render::Rect BOX = { .x = 0.0F, .y = 0.0F, .width = 300.0F, .height = 40.0F };
    constexpr RType::Render::Vec2D INSIDE = { .x = 150.0F, .y = 20.0F };
    constexpr RType::Render::Vec2D OUTSIDE = { .x = 150.0F, .y = 100.0F };

    FrameInput idle(RType::Render::Vec2D mouse = OUTSIDE)
    {
        FrameInput input;

        input.mouse = mouse;
        return input;
    }

    FrameInput pressed(MenuAction action)
    {
        FrameInput input = idle();

        input.press(action);
        return input;
    }

    FrameInput clicked(RType::Render::Vec2D mouse)
    {
        FrameInput input = idle(mouse);

        input.click = true;
        input.mouseDown = true;
        return input;
    }
}


TEST(ui_Checkbox, keepsItsInitialState)
{
    EXPECT_TRUE(Checkbox("Fullscreen", true, BOX).checked());
    EXPECT_FALSE(Checkbox("Fullscreen", false, BOX).checked());
}

TEST(ui_Checkbox, confirmToggles)
{
    Checkbox box("Fullscreen", false, BOX);

    EXPECT_TRUE(box.update(pressed(MenuAction::Confirm)));
    EXPECT_TRUE(box.checked());
    EXPECT_TRUE(box.update(pressed(MenuAction::Confirm)));
    EXPECT_FALSE(box.checked());
}

TEST(ui_Checkbox, clickInsideToggles)
{
    Checkbox box("Fullscreen", false, BOX);

    EXPECT_TRUE(box.update(clicked(INSIDE)));
    EXPECT_TRUE(box.checked());
}

TEST(ui_Checkbox, clickOutsideDoesNothing)
{
    Checkbox box("Fullscreen", false, BOX);

    EXPECT_FALSE(box.update(clicked(OUTSIDE)));
    EXPECT_FALSE(box.checked());
}

TEST(ui_Checkbox, heldButtonDoesNotToggleAgain)
{
    Checkbox box("Fullscreen", false, BOX);
    FrameInput held = idle(INSIDE);

    held.mouseDown = true;
    box.update(clicked(INSIDE));
    EXPECT_FALSE(box.update(held));
    EXPECT_TRUE(box.checked());
}

TEST(ui_Checkbox, leftSetsOffAndRightSetsOn)
{
    Checkbox box("Fullscreen", false, BOX);

    EXPECT_TRUE(box.update(pressed(MenuAction::Right)));
    EXPECT_TRUE(box.checked());
    EXPECT_TRUE(box.update(pressed(MenuAction::Left)));
    EXPECT_FALSE(box.checked());
}

TEST(ui_Checkbox, leftOrRightOnTheSameStateIsNotAChange)
{
    Checkbox on("Fullscreen", true, BOX);
    Checkbox off("Fullscreen", false, BOX);

    EXPECT_FALSE(on.update(pressed(MenuAction::Right)));
    EXPECT_TRUE(on.checked());
    EXPECT_FALSE(off.update(pressed(MenuAction::Left)));
    EXPECT_FALSE(off.checked());
}

TEST(ui_Checkbox, otherActionsDoNothing)
{
    Checkbox box("Fullscreen", false, BOX);

    for (MenuAction action : {MenuAction::Up, MenuAction::Down, MenuAction::Back})
        EXPECT_FALSE(box.update(pressed(action)));
    EXPECT_FALSE(box.checked());
}

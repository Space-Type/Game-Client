/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** Slider class
*/

#include <gtest/gtest.h>
#include <rtype/ui.hpp>

namespace {

    using RType::UI::FrameInput;
    using RType::UI::MenuAction;
    using RType::UI::Slider;

    // A 0-100 slider by steps of 5, in a box from x = 0 to x = 324
    constexpr RType::Render::Rect BOX = { .x = 0.0F, .y = 0.0F, .width = 324.0F, .height = 40.0F };

    Slider makeSlider(int value = 50)
    {
        return Slider("Volume", value, 0, 100, 5, BOX, "%");
    }

    FrameInput idle(float x = 1000.0F, float y = 20.0F)
    {
        FrameInput input;

        input.mouse = { .x = x, .y = y };
        return input;
    }

    FrameInput pressed(MenuAction action)
    {
        FrameInput input = idle();

        input.press(action);
        return input;
    }

    FrameInput clickedAt(float x, float y = 20.0F)
    {
        FrameInput input = idle(x, y);

        input.click = true;
        input.mouseDown = true;
        return input;
    }

    FrameInput heldAt(float x, float y = 20.0F)
    {
        FrameInput input = idle(x, y);

        input.mouseDown = true;
        return input;
    }

    // x on the track for a ratio between 0 and 1
    float trackX(const Slider& slider, float ratio)
    {
        RType::Render::Rect track = slider.track();

        return track.x + track.width * ratio;
    }
}


TEST(ui_Slider, keepsAValueInRange)
{
    EXPECT_EQ(makeSlider(150).value(), 100);
    EXPECT_EQ(makeSlider(-20).value(), 0);
}

TEST(ui_Slider, roundsAValueToTheNearestStep)
{
    EXPECT_EQ(makeSlider(52).value(), 50);
    EXPECT_EQ(makeSlider(53).value(), 55);
}

TEST(ui_Slider, stepsFromTheMinimum)
{
    Slider slider("Level", 4, 1, 10, 3, BOX);

    EXPECT_EQ(slider.value(), 4);
    EXPECT_TRUE(slider.update(pressed(MenuAction::Right)));
    EXPECT_EQ(slider.value(), 7);
}

TEST(ui_Slider, invalidRangeAndStepAreHarmless)
{
    Slider empty("Empty", 3, 10, 0, 0, BOX);

    EXPECT_EQ(empty.value(), 10);
    EXPECT_FALSE(empty.update(pressed(MenuAction::Right)));
    EXPECT_FALSE(empty.update(clickedAt(trackX(empty, 0.5F))));
    EXPECT_EQ(empty.value(), 10);
}

TEST(ui_Slider, rightAndLeftMoveByOneStep)
{
    Slider slider = makeSlider();

    EXPECT_TRUE(slider.update(pressed(MenuAction::Right)));
    EXPECT_EQ(slider.value(), 55);
    EXPECT_TRUE(slider.update(pressed(MenuAction::Left)));
    EXPECT_TRUE(slider.update(pressed(MenuAction::Left)));
    EXPECT_EQ(slider.value(), 45);
}

TEST(ui_Slider, boundsAreNotAChange)
{
    Slider high = makeSlider(100);
    Slider low = makeSlider(0);

    EXPECT_FALSE(high.update(pressed(MenuAction::Right)));
    EXPECT_EQ(high.value(), 100);
    EXPECT_FALSE(low.update(pressed(MenuAction::Left)));
    EXPECT_EQ(low.value(), 0);
}

TEST(ui_Slider, otherActionsDoNothing)
{
    Slider slider = makeSlider();

    for (MenuAction action : {MenuAction::Up, MenuAction::Down, MenuAction::Confirm, MenuAction::Back})
        EXPECT_FALSE(slider.update(pressed(action)));
    EXPECT_EQ(slider.value(), 50);
}

TEST(ui_Slider, trackLeavesRoomForTheValue)
{
    RType::Render::Rect track = makeSlider().track();

    EXPECT_GT(track.x, BOX.x);
    EXPECT_LT(track.x + track.width, BOX.x + BOX.width - RType::UI::Theme::VALUE_WIDTH);
    EXPECT_GT(track.width, 0.0F);
}

TEST(ui_Slider, clickOnTheTrackSetsTheValue)
{
    Slider slider = makeSlider();

    EXPECT_TRUE(slider.update(clickedAt(trackX(slider, 0.8F))));
    EXPECT_EQ(slider.value(), 80);
    EXPECT_TRUE(slider.dragging());
}

TEST(ui_Slider, dragFollowsTheMouseOnSteps)
{
    Slider slider = makeSlider();

    slider.update(clickedAt(trackX(slider, 0.5F)));
    EXPECT_TRUE(slider.update(heldAt(trackX(slider, 0.21F))));
    EXPECT_EQ(slider.value(), 20);
    EXPECT_TRUE(slider.update(heldAt(trackX(slider, 0.23F))));
    EXPECT_EQ(slider.value(), 25);
}

TEST(ui_Slider, dragGoesOnOutsideTheBox)
{
    Slider slider = makeSlider();

    slider.update(clickedAt(trackX(slider, 0.5F)));
    slider.update(heldAt(-500.0F, 300.0F));
    EXPECT_EQ(slider.value(), 0);
    slider.update(heldAt(5000.0F, -300.0F));
    EXPECT_EQ(slider.value(), 100);
}

TEST(ui_Slider, releaseEndsTheDrag)
{
    Slider slider = makeSlider();

    slider.update(clickedAt(trackX(slider, 0.5F)));
    slider.update(idle(trackX(slider, 0.5F)));
    EXPECT_FALSE(slider.dragging());
    EXPECT_FALSE(slider.update(idle(trackX(slider, 0.9F))));
    EXPECT_EQ(slider.value(), 50);
}

TEST(ui_Slider, heldButtonWithoutClickDoesNotDrag)
{
    Slider slider = makeSlider();

    EXPECT_FALSE(slider.update(heldAt(trackX(slider, 0.9F))));
    EXPECT_FALSE(slider.dragging());
    EXPECT_EQ(slider.value(), 50);
}

TEST(ui_Slider, clickOutsideTheBoxDoesNotDrag)
{
    Slider slider = makeSlider();

    EXPECT_FALSE(slider.update(clickedAt(trackX(slider, 0.9F), 100.0F)));
    EXPECT_FALSE(slider.dragging());
}

TEST(ui_Slider, clickOnTheValueKeepsItInRange)
{
    Slider slider = makeSlider();

    slider.update(clickedAt(BOX.width - 10.0F));
    EXPECT_EQ(slider.value(), 100);
}
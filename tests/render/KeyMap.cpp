/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** Render <-> raylib input mappings
*/

#include <gtest/gtest.h>
#include "../../src/render/Input/KeyMap.hpp"
#include <cstddef>
#include <limits>
#include <set>

namespace Detail = RType::Render::Detail;

namespace {
    template <std::size_t N>
    bool allUnique(const std::array<int, N>& table)
    {
        return std::set<int>(table.begin(), table.end()).size() == N;
    }
}


/////////////////////////////////////////////////////////
//                                                     //
//                       KEYBOARD                      //
//                                                     //
/////////////////////////////////////////////////////////

TEST(render_KeyMap, keysAreUniqueAndNeverNull)
{
    EXPECT_TRUE(allUnique(Detail::KEYS));
    for (int key : Detail::KEYS)
        EXPECT_NE(key, KEY_NULL);
}

TEST(render_KeyMap, keyRoundTrip)
{
    for (std::size_t i = 0; i < Detail::COUNT_OF<RType::Render::Key>; i++) {
        auto key = static_cast<RType::Render::Key>(i);
        std::optional<RType::Render::Key> back = Detail::fromRaylibKey(Detail::toRaylib(key));

        ASSERT_TRUE(back.has_value()) << "key index " << i;
        EXPECT_EQ(*back, key) << "key index " << i;
    }
}

TEST(render_KeyMap, keyRanges)
{
    for (int i = 0; i < 26; i++)
        EXPECT_EQ(Detail::toRaylib(static_cast<RType::Render::Key>(static_cast<int>(RType::Render::Key::A) + i)), KEY_A + i);
    for (int i = 0; i < 10; i++)
        EXPECT_EQ(Detail::toRaylib(static_cast<RType::Render::Key>(static_cast<int>(RType::Render::Key::Num0) + i)), KEY_ZERO + i);
    for (int i = 0; i < 12; i++)
        EXPECT_EQ(Detail::toRaylib(static_cast<RType::Render::Key>(static_cast<int>(RType::Render::Key::F1) + i)), KEY_F1 + i);
    for (int i = 0; i < 10; i++)
        EXPECT_EQ(Detail::toRaylib(static_cast<RType::Render::Key>(static_cast<int>(RType::Render::Key::Kp0) + i)), KEY_KP_0 + i);
}

TEST(render_KeyMap, keyExamples)
{
    EXPECT_EQ(Detail::toRaylib(RType::Render::Key::Up), KEY_UP);
    EXPECT_EQ(Detail::toRaylib(RType::Render::Key::Down), KEY_DOWN);
    EXPECT_EQ(Detail::toRaylib(RType::Render::Key::Left), KEY_LEFT);
    EXPECT_EQ(Detail::toRaylib(RType::Render::Key::Right), KEY_RIGHT);
    EXPECT_EQ(Detail::toRaylib(RType::Render::Key::Space), KEY_SPACE);
    EXPECT_EQ(Detail::toRaylib(RType::Render::Key::Escape), KEY_ESCAPE);
    EXPECT_EQ(Detail::toRaylib(RType::Render::Key::RightAlt), KEY_RIGHT_ALT);
    EXPECT_EQ(Detail::toRaylib(RType::Render::Key::Grave), KEY_GRAVE);
    EXPECT_EQ(Detail::toRaylib(RType::Render::Key::KpDecimal), KEY_KP_DECIMAL);
}

TEST(render_KeyMap, keySentinels)
{
    EXPECT_EQ(Detail::toRaylib(RType::Render::Key::Count), KEY_NULL);
    EXPECT_EQ(Detail::toRaylib(static_cast<RType::Render::Key>(255)), KEY_NULL);
    EXPECT_EQ(Detail::indexOf(RType::Render::Key::Count), Detail::COUNT_OF<RType::Render::Key>);
    EXPECT_EQ(Detail::indexOf(static_cast<RType::Render::Key>(200)), Detail::COUNT_OF<RType::Render::Key>);
}

TEST(render_KeyMap, unmappedKeysAreNullopt)
{
    EXPECT_FALSE(Detail::fromRaylibKey(KEY_NULL).has_value());
    EXPECT_FALSE(Detail::fromRaylibKey(KEY_CAPS_LOCK).has_value());
    EXPECT_FALSE(Detail::fromRaylibKey(KEY_MENU).has_value());
    EXPECT_FALSE(Detail::fromRaylibKey(KEY_PRINT_SCREEN).has_value());
    EXPECT_FALSE(Detail::fromRaylibKey(-1).has_value());
    EXPECT_FALSE(Detail::fromRaylibKey(100000).has_value());
}


/////////////////////////////////////////////////////////
//                                                     //
//                     MOUSE / GAMEPAD                 //
//                                                     //
/////////////////////////////////////////////////////////

TEST(render_KeyMap, mouseButtons)
{
    EXPECT_TRUE(allUnique(Detail::MOUSE_BUTTONS));
    EXPECT_EQ(Detail::toRaylib(RType::Render::MouseButton::Left), MOUSE_BUTTON_LEFT);
    EXPECT_EQ(Detail::toRaylib(RType::Render::MouseButton::Right), MOUSE_BUTTON_RIGHT);
    EXPECT_EQ(Detail::toRaylib(RType::Render::MouseButton::Middle), MOUSE_BUTTON_MIDDLE);
    EXPECT_EQ(Detail::toRaylib(RType::Render::MouseButton::Count), -1);
    EXPECT_EQ(Detail::toRaylib(static_cast<RType::Render::MouseButton>(255)), -1);
}

TEST(render_KeyMap, gamepadButtons)
{
    EXPECT_TRUE(allUnique(Detail::GAMEPAD_BUTTONS));
    for (int button : Detail::GAMEPAD_BUTTONS)
        EXPECT_NE(button, GAMEPAD_BUTTON_UNKNOWN);
    EXPECT_EQ(Detail::toRaylib(RType::Render::GamepadButton::DpadUp), GAMEPAD_BUTTON_LEFT_FACE_UP);
    EXPECT_EQ(Detail::toRaylib(RType::Render::GamepadButton::DpadLeft), GAMEPAD_BUTTON_LEFT_FACE_LEFT);
    EXPECT_EQ(Detail::toRaylib(RType::Render::GamepadButton::A), GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
    EXPECT_EQ(Detail::toRaylib(RType::Render::GamepadButton::Y), GAMEPAD_BUTTON_RIGHT_FACE_UP);
    EXPECT_EQ(Detail::toRaylib(RType::Render::GamepadButton::LeftBumper), GAMEPAD_BUTTON_LEFT_TRIGGER_1);
    EXPECT_EQ(Detail::toRaylib(RType::Render::GamepadButton::RightTrigger), GAMEPAD_BUTTON_RIGHT_TRIGGER_2);
    EXPECT_EQ(Detail::toRaylib(RType::Render::GamepadButton::Start), GAMEPAD_BUTTON_MIDDLE_RIGHT);
    EXPECT_EQ(Detail::toRaylib(RType::Render::GamepadButton::RightThumb), GAMEPAD_BUTTON_RIGHT_THUMB);
    EXPECT_EQ(Detail::toRaylib(RType::Render::GamepadButton::Count), GAMEPAD_BUTTON_UNKNOWN);
    EXPECT_EQ(Detail::toRaylib(static_cast<RType::Render::GamepadButton>(255)), GAMEPAD_BUTTON_UNKNOWN);
}

TEST(render_KeyMap, gamepadAxes)
{
    EXPECT_TRUE(allUnique(Detail::GAMEPAD_AXES));
    EXPECT_EQ(Detail::toRaylib(RType::Render::GamepadAxis::LeftX), GAMEPAD_AXIS_LEFT_X);
    EXPECT_EQ(Detail::toRaylib(RType::Render::GamepadAxis::LeftY), GAMEPAD_AXIS_LEFT_Y);
    EXPECT_EQ(Detail::toRaylib(RType::Render::GamepadAxis::RightX), GAMEPAD_AXIS_RIGHT_X);
    EXPECT_EQ(Detail::toRaylib(RType::Render::GamepadAxis::RightY), GAMEPAD_AXIS_RIGHT_Y);
    EXPECT_EQ(Detail::toRaylib(RType::Render::GamepadAxis::LeftTrigger), GAMEPAD_AXIS_LEFT_TRIGGER);
    EXPECT_EQ(Detail::toRaylib(RType::Render::GamepadAxis::RightTrigger), GAMEPAD_AXIS_RIGHT_TRIGGER);
    EXPECT_EQ(Detail::toRaylib(RType::Render::GamepadAxis::Count), -1);
    EXPECT_EQ(Detail::toRaylib(static_cast<RType::Render::GamepadAxis>(255)), -1);
}

TEST(render_KeyMap, axisNormalization)
{
    using RType::Render::GamepadAxis;
    constexpr float NaN = std::numeric_limits<float>::quiet_NaN();

    EXPECT_FLOAT_EQ(Detail::normalizeAxis(GamepadAxis::LeftX, -0.5F), -0.5F);
    EXPECT_FLOAT_EQ(Detail::normalizeAxis(GamepadAxis::RightY, 3.0F), 1.0F);
    EXPECT_FLOAT_EQ(Detail::normalizeAxis(GamepadAxis::LeftY, -3.0F), -1.0F);
    EXPECT_FLOAT_EQ(Detail::normalizeAxis(GamepadAxis::LeftTrigger, -1.0F), 0.0F);
    EXPECT_FLOAT_EQ(Detail::normalizeAxis(GamepadAxis::LeftTrigger, 0.0F), 0.5F);
    EXPECT_FLOAT_EQ(Detail::normalizeAxis(GamepadAxis::RightTrigger, 1.0F), 1.0F);
    EXPECT_FLOAT_EQ(Detail::normalizeAxis(GamepadAxis::RightTrigger, -2.0F), 0.0F);
    EXPECT_FLOAT_EQ(Detail::normalizeAxis(GamepadAxis::RightTrigger, 2.0F), 1.0F);
    EXPECT_FLOAT_EQ(Detail::normalizeAxis(GamepadAxis::LeftX, NaN), 0.0F);
    EXPECT_FLOAT_EQ(Detail::normalizeAxis(GamepadAxis::LeftTrigger, NaN), 0.0F);
    EXPECT_FLOAT_EQ(Detail::normalizeAxis(GamepadAxis::Count, 0.5F), 0.0F);
}

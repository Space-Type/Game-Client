/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Unit tests for
** backend <-> raylib input mappings
*/

#include <gtest/gtest.h>
#include "Input/KeyMap.hpp"
#include <cstddef>
#include <limits>
#include <set>

namespace Detail = Backend::Detail;

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

TEST(backend_KeyMap, keysAreUniqueAndNeverNull)
{
    EXPECT_TRUE(allUnique(Detail::KEYS));
    for (int key : Detail::KEYS)
        EXPECT_NE(key, KEY_NULL);
}

TEST(backend_KeyMap, keyRoundTrip)
{
    for (std::size_t i = 0; i < Detail::COUNT_OF<Backend::Key>; i++) {
        auto key = static_cast<Backend::Key>(i);
        std::optional<Backend::Key> back = Detail::fromRaylibKey(Detail::toRaylib(key));

        ASSERT_TRUE(back.has_value()) << "key index " << i;
        EXPECT_EQ(*back, key) << "key index " << i;
    }
}

TEST(backend_KeyMap, keyRanges)
{
    for (int i = 0; i < 26; i++)
        EXPECT_EQ(Detail::toRaylib(static_cast<Backend::Key>(static_cast<int>(Backend::Key::A) + i)), KEY_A + i);
    for (int i = 0; i < 10; i++)
        EXPECT_EQ(Detail::toRaylib(static_cast<Backend::Key>(static_cast<int>(Backend::Key::Num0) + i)), KEY_ZERO + i);
    for (int i = 0; i < 12; i++)
        EXPECT_EQ(Detail::toRaylib(static_cast<Backend::Key>(static_cast<int>(Backend::Key::F1) + i)), KEY_F1 + i);
    for (int i = 0; i < 10; i++)
        EXPECT_EQ(Detail::toRaylib(static_cast<Backend::Key>(static_cast<int>(Backend::Key::Kp0) + i)), KEY_KP_0 + i);
}

TEST(backend_KeyMap, keyExamples)
{
    EXPECT_EQ(Detail::toRaylib(Backend::Key::Up), KEY_UP);
    EXPECT_EQ(Detail::toRaylib(Backend::Key::Down), KEY_DOWN);
    EXPECT_EQ(Detail::toRaylib(Backend::Key::Left), KEY_LEFT);
    EXPECT_EQ(Detail::toRaylib(Backend::Key::Right), KEY_RIGHT);
    EXPECT_EQ(Detail::toRaylib(Backend::Key::Space), KEY_SPACE);
    EXPECT_EQ(Detail::toRaylib(Backend::Key::Escape), KEY_ESCAPE);
    EXPECT_EQ(Detail::toRaylib(Backend::Key::RightAlt), KEY_RIGHT_ALT);
    EXPECT_EQ(Detail::toRaylib(Backend::Key::Grave), KEY_GRAVE);
    EXPECT_EQ(Detail::toRaylib(Backend::Key::KpDecimal), KEY_KP_DECIMAL);
}

TEST(backend_KeyMap, keySentinels)
{
    EXPECT_EQ(Detail::toRaylib(Backend::Key::Count), KEY_NULL);
    EXPECT_EQ(Detail::toRaylib(static_cast<Backend::Key>(255)), KEY_NULL);
    EXPECT_EQ(Detail::indexOf(Backend::Key::Count), Detail::COUNT_OF<Backend::Key>);
    EXPECT_EQ(Detail::indexOf(static_cast<Backend::Key>(200)), Detail::COUNT_OF<Backend::Key>);
}

TEST(backend_KeyMap, unmappedKeysAreNullopt)
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

TEST(backend_KeyMap, mouseButtons)
{
    EXPECT_TRUE(allUnique(Detail::MOUSE_BUTTONS));
    EXPECT_EQ(Detail::toRaylib(Backend::MouseButton::Left), MOUSE_BUTTON_LEFT);
    EXPECT_EQ(Detail::toRaylib(Backend::MouseButton::Right), MOUSE_BUTTON_RIGHT);
    EXPECT_EQ(Detail::toRaylib(Backend::MouseButton::Middle), MOUSE_BUTTON_MIDDLE);
    EXPECT_EQ(Detail::toRaylib(Backend::MouseButton::Count), -1);
    EXPECT_EQ(Detail::toRaylib(static_cast<Backend::MouseButton>(255)), -1);
}

TEST(backend_KeyMap, gamepadButtons)
{
    EXPECT_TRUE(allUnique(Detail::GAMEPAD_BUTTONS));
    for (int button : Detail::GAMEPAD_BUTTONS)
        EXPECT_NE(button, GAMEPAD_BUTTON_UNKNOWN);
    EXPECT_EQ(Detail::toRaylib(Backend::GamepadButton::DpadUp), GAMEPAD_BUTTON_LEFT_FACE_UP);
    EXPECT_EQ(Detail::toRaylib(Backend::GamepadButton::DpadLeft), GAMEPAD_BUTTON_LEFT_FACE_LEFT);
    EXPECT_EQ(Detail::toRaylib(Backend::GamepadButton::A), GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
    EXPECT_EQ(Detail::toRaylib(Backend::GamepadButton::Y), GAMEPAD_BUTTON_RIGHT_FACE_UP);
    EXPECT_EQ(Detail::toRaylib(Backend::GamepadButton::LeftBumper), GAMEPAD_BUTTON_LEFT_TRIGGER_1);
    EXPECT_EQ(Detail::toRaylib(Backend::GamepadButton::RightTrigger), GAMEPAD_BUTTON_RIGHT_TRIGGER_2);
    EXPECT_EQ(Detail::toRaylib(Backend::GamepadButton::Start), GAMEPAD_BUTTON_MIDDLE_RIGHT);
    EXPECT_EQ(Detail::toRaylib(Backend::GamepadButton::RightThumb), GAMEPAD_BUTTON_RIGHT_THUMB);
    EXPECT_EQ(Detail::toRaylib(Backend::GamepadButton::Count), GAMEPAD_BUTTON_UNKNOWN);
    EXPECT_EQ(Detail::toRaylib(static_cast<Backend::GamepadButton>(255)), GAMEPAD_BUTTON_UNKNOWN);
}

TEST(backend_KeyMap, gamepadAxes)
{
    EXPECT_TRUE(allUnique(Detail::GAMEPAD_AXES));
    EXPECT_EQ(Detail::toRaylib(Backend::GamepadAxis::LeftX), GAMEPAD_AXIS_LEFT_X);
    EXPECT_EQ(Detail::toRaylib(Backend::GamepadAxis::LeftY), GAMEPAD_AXIS_LEFT_Y);
    EXPECT_EQ(Detail::toRaylib(Backend::GamepadAxis::RightX), GAMEPAD_AXIS_RIGHT_X);
    EXPECT_EQ(Detail::toRaylib(Backend::GamepadAxis::RightY), GAMEPAD_AXIS_RIGHT_Y);
    EXPECT_EQ(Detail::toRaylib(Backend::GamepadAxis::LeftTrigger), GAMEPAD_AXIS_LEFT_TRIGGER);
    EXPECT_EQ(Detail::toRaylib(Backend::GamepadAxis::RightTrigger), GAMEPAD_AXIS_RIGHT_TRIGGER);
    EXPECT_EQ(Detail::toRaylib(Backend::GamepadAxis::Count), -1);
    EXPECT_EQ(Detail::toRaylib(static_cast<Backend::GamepadAxis>(255)), -1);
}

TEST(backend_KeyMap, axisNormalization)
{
    using Backend::GamepadAxis;
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

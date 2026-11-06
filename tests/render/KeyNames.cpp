/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** human readable key names
*/

#include <gtest/gtest.h>
#include <rtype/render.hpp>
#include <cstddef>
#include <set>
#include <string_view>

namespace {
    constexpr std::size_t KEY_COUNT = static_cast<std::size_t>(RType::Render::Key::Count);
}

TEST(render_KeyNames, everyKeyHasAName)
{
    for (std::size_t i = 0; i < KEY_COUNT; i++) {
        std::string_view name = RType::Render::keyName(static_cast<RType::Render::Key>(i));

        EXPECT_FALSE(name.empty()) << "key index " << i;
        EXPECT_NE(name, "Unknown") << "key index " << i;
    }
}

TEST(render_KeyNames, namesAreUnique)
{
    std::set<std::string_view> names;

    for (std::size_t i = 0; i < KEY_COUNT; i++)
        EXPECT_TRUE(names.insert(RType::Render::keyName(static_cast<RType::Render::Key>(i))).second)
            << "duplicate name for key index " << i;
}

TEST(render_KeyNames, countAndOutOfRangeAreUnknown)
{
    EXPECT_EQ(RType::Render::keyName(RType::Render::Key::Count), "Unknown");
    EXPECT_EQ(RType::Render::keyName(static_cast<RType::Render::Key>(KEY_COUNT + 1)), "Unknown");
    EXPECT_EQ(RType::Render::keyName(static_cast<RType::Render::Key>(255)), "Unknown");
}

TEST(render_KeyNames, examples)
{
    EXPECT_EQ(RType::Render::keyName(RType::Render::Key::A), "A");
    EXPECT_EQ(RType::Render::keyName(RType::Render::Key::Z), "Z");
    EXPECT_EQ(RType::Render::keyName(RType::Render::Key::Num0), "0");
    EXPECT_EQ(RType::Render::keyName(RType::Render::Key::F5), "F5");
    EXPECT_EQ(RType::Render::keyName(RType::Render::Key::F12), "F12");
    EXPECT_EQ(RType::Render::keyName(RType::Render::Key::Up), "Up");
    EXPECT_EQ(RType::Render::keyName(RType::Render::Key::Space), "Space");
    EXPECT_EQ(RType::Render::keyName(RType::Render::Key::LeftShift), "Left Shift");
    EXPECT_EQ(RType::Render::keyName(RType::Render::Key::Grave), "`");
    EXPECT_EQ(RType::Render::keyName(RType::Render::Key::Kp9), "Keypad 9");
    EXPECT_EQ(RType::Render::keyName(RType::Render::Key::KpDecimal), "Keypad .");
}

TEST(render_KeyNames, mouseAndGamepadNames)
{
    using RType::Render::GamepadButton;
    using RType::Render::MouseButton;

    EXPECT_EQ(RType::Render::mouseButtonName(MouseButton::Left), "Mouse Left");
    EXPECT_EQ(RType::Render::mouseButtonName(MouseButton::Count), "Unknown");
    EXPECT_EQ(RType::Render::gamepadButtonName(GamepadButton::DpadUp), "D-pad Up");
    EXPECT_EQ(RType::Render::gamepadButtonName(GamepadButton::A), "A");
    EXPECT_EQ(RType::Render::gamepadButtonName(GamepadButton::RightThumb), "Right Stick");
    EXPECT_EQ(RType::Render::gamepadButtonName(GamepadButton::Count), "Unknown");
}

TEST(render_KeyNames, everyNameParsesBack)
{
    using namespace RType::Render;

    for (std::size_t i = 0; i < KEY_COUNT; i++)
        EXPECT_EQ(parseKey(keyName(static_cast<Key>(i))), static_cast<Key>(i)) << "key index " << i;
    for (std::size_t i = 0; i < static_cast<std::size_t>(MouseButton::Count); i++)
        EXPECT_EQ(parseMouseButton(mouseButtonName(static_cast<MouseButton>(i))), static_cast<MouseButton>(i));
    for (std::size_t i = 0; i < static_cast<std::size_t>(GamepadButton::Count); i++)
        EXPECT_EQ(parseGamepadButton(gamepadButtonName(static_cast<GamepadButton>(i))), static_cast<GamepadButton>(i));
}

TEST(render_KeyNames, unknownNamesDoNotParse)
{
    using namespace RType::Render;

    EXPECT_EQ(parseKey(""), std::nullopt);
    EXPECT_EQ(parseKey("Unknown"), std::nullopt);
    EXPECT_EQ(parseKey("space"), std::nullopt);       // exact names only
    EXPECT_EQ(parseKey("Mouse Left"), std::nullopt);
    EXPECT_EQ(parseMouseButton("Left"), std::nullopt);
    EXPECT_EQ(parseGamepadButton("D-pad"), std::nullopt);
}

/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Unit tests for
** human readable key names
*/

#include <gtest/gtest.h>
#include <backend/InputTypes.hpp>
#include <cstddef>
#include <set>
#include <string_view>

namespace {
    constexpr std::size_t KEY_COUNT = static_cast<std::size_t>(Backend::Key::Count);
}

TEST(backend_KeyNames, everyKeyHasAName)
{
    for (std::size_t i = 0; i < KEY_COUNT; i++) {
        std::string_view name = Backend::keyName(static_cast<Backend::Key>(i));

        EXPECT_FALSE(name.empty()) << "key index " << i;
        EXPECT_NE(name, "Unknown") << "key index " << i;
    }
}

TEST(backend_KeyNames, namesAreUnique)
{
    std::set<std::string_view> names;

    for (std::size_t i = 0; i < KEY_COUNT; i++)
        EXPECT_TRUE(names.insert(Backend::keyName(static_cast<Backend::Key>(i))).second)
            << "duplicate name for key index " << i;
}

TEST(backend_KeyNames, countAndOutOfRangeAreUnknown)
{
    EXPECT_EQ(Backend::keyName(Backend::Key::Count), "Unknown");
    EXPECT_EQ(Backend::keyName(static_cast<Backend::Key>(KEY_COUNT + 1)), "Unknown");
    EXPECT_EQ(Backend::keyName(static_cast<Backend::Key>(255)), "Unknown");
}

TEST(backend_KeyNames, examples)
{
    EXPECT_EQ(Backend::keyName(Backend::Key::A), "A");
    EXPECT_EQ(Backend::keyName(Backend::Key::Z), "Z");
    EXPECT_EQ(Backend::keyName(Backend::Key::Num0), "0");
    EXPECT_EQ(Backend::keyName(Backend::Key::F5), "F5");
    EXPECT_EQ(Backend::keyName(Backend::Key::F12), "F12");
    EXPECT_EQ(Backend::keyName(Backend::Key::Up), "Up");
    EXPECT_EQ(Backend::keyName(Backend::Key::Space), "Space");
    EXPECT_EQ(Backend::keyName(Backend::Key::LeftShift), "Left Shift");
    EXPECT_EQ(Backend::keyName(Backend::Key::Grave), "`");
    EXPECT_EQ(Backend::keyName(Backend::Key::Kp9), "Keypad 9");
    EXPECT_EQ(Backend::keyName(Backend::Key::KpDecimal), "Keypad .");
}

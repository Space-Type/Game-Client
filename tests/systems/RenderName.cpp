/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** RenderName system
*/

#include <gtest/gtest.h>
#include <rtype/systems.hpp>
#include <string>

TEST(systems_RenderName, printableNameKeepsAscii)
{
    EXPECT_EQ(RType::Systems::printableName("Bard_42 !~"), "Bard_42 !~");
    EXPECT_EQ(RType::Systems::printableName(""), "");
}

TEST(systems_RenderName, printableNameReplacesTheRest)
{
    EXPECT_EQ(RType::Systems::printableName("A\nB\tC"), "A?B?C");
    EXPECT_EQ(RType::Systems::printableName(std::string("x\0y", 3)), "x?y");
    EXPECT_EQ(RType::Systems::printableName("\x7f\xff"), "??");
    EXPECT_EQ(RType::Systems::printableName("é"), "??");  // two UTF-8 bytes
}

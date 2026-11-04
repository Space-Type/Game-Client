/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** TextField class
*/

#include <gtest/gtest.h>
#include <rtype/ui.hpp>

namespace {

    using RType::UI::FrameInput;
    using RType::UI::TextField;

    FrameInput typed(std::u32string text)
    {
        FrameInput input;

        input.text = std::move(text);
        return input;
    }

    FrameInput backspace()
    {
        FrameInput input;

        input.backspace = true;
        return input;
    }

    TextField makeField(std::string value = "", std::size_t maxLength = 8, TextField::Filter filter = nullptr)
    {
        return TextField("Label", std::move(value), maxLength, {}, filter);
    }
}


TEST(ui_TextField, keepsItsInitialValue)
{
    EXPECT_EQ(makeField("abc").value(), "abc");
}

TEST(ui_TextField, typingAppends)
{
    TextField field = makeField("a");

    field.update(typed(U"bc"));
    field.update(typed(U"d"));
    EXPECT_EQ(field.value(), "abcd");
}

TEST(ui_TextField, stopsAtMaxLength)
{
    TextField field = makeField("", 3);

    field.update(typed(U"abcde"));
    EXPECT_EQ(field.value(), "abc");
    field.update(typed(U"f"));
    EXPECT_EQ(field.value(), "abc");
}

TEST(ui_TextField, onlyAcceptsPrintableAscii)
{
    TextField field = makeField("", 32);

    field.update(typed(U"\t\n\r\x1F \x7Fé€\U0001F600~"));
    EXPECT_EQ(field.value(), " ~");
}

TEST(ui_TextField, filterRestrictsCharacters)
{
    TextField field = makeField("", 8, RType::UI::isDigit);

    field.update(typed(U"1a2-3 4"));
    EXPECT_EQ(field.value(), "1234");
}

TEST(ui_TextField, backspaceErasesTheEnd)
{
    TextField field = makeField("abc");

    field.update(backspace());
    EXPECT_EQ(field.value(), "ab");
}

TEST(ui_TextField, backspaceOnEmptyIsNoop)
{
    TextField field = makeField();

    field.update(backspace());
    EXPECT_EQ(field.value(), "");
}

TEST(ui_TextField, tooLongInitialValueCanBeErased)
{
    TextField field = makeField("abcdef", 3);

    field.update(typed(U"x"));
    EXPECT_EQ(field.value(), "abcdef");
    field.update(backspace());
    EXPECT_EQ(field.value(), "abcde");
}

TEST(ui_TextField, withoutInputNothingChanges)
{
    TextField field = makeField("abc");

    field.update(FrameInput{});
    EXPECT_EQ(field.value(), "abc");
}

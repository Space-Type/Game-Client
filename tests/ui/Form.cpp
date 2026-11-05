/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** Form class
*/

#include <gtest/gtest.h>
#include <rtype/ui.hpp>
#include <optional>
#include <variant>
#include <vector>

namespace {

    using RType::UI::Button;
    using RType::UI::Form;
    using RType::UI::FrameInput;
    using RType::UI::MenuAction;
    using RType::UI::TextField;

    constexpr std::size_t FIRST_FIELD = 0;
    constexpr std::size_t SECOND_FIELD = 1;
    constexpr std::size_t OK_BUTTON = 2;
    constexpr std::size_t QUIT_BUTTON = 3;

    constexpr RType::Render::Vec2D OUTSIDE = { .x = 1000.0F, .y = 1000.0F };
    constexpr RType::Render::Vec2D ON_FIRST_FIELD = { .x = 50.0F, .y = 5.0F };
    constexpr RType::Render::Vec2D ON_SECOND_FIELD = { .x = 50.0F, .y = 25.0F };
    constexpr RType::Render::Vec2D ON_OK_BUTTON = { .x = 50.0F, .y = 45.0F };

    constexpr RType::Render::Rect row(float y)
    {
        return { .x = 0.0F, .y = y, .width = 100.0F, .height = 10.0F };
    }

    Form makeForm()
    {
        return Form(std::vector<RType::UI::Widget>{
            TextField("First", "", 8, row(0.0F)),
            TextField("Second", "", 8, row(20.0F)),
            Button("Ok", row(40.0F)),
            Button("Quit", row(60.0F)),
        });
    }

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
        return input;
    }

    std::optional<RType::UI::FormEvent> activated(std::size_t index)
    {
        return RType::UI::FormEvent{ .kind = RType::UI::FormEvent::Kind::Activated, .index = index };
    }

    std::string valueOf(const Form& form, std::size_t field)
    {
        return std::get<TextField>(form.widgets()[field]).value();
    }
}


TEST(ui_Form, focusStartsOnFirstWidget)
{
    EXPECT_EQ(makeForm().focus(), FIRST_FIELD);
}

TEST(ui_Form, emptyFormIsHarmless)
{
    Form form{std::vector<RType::UI::Widget>{}};

    EXPECT_EQ(form.update(pressed(MenuAction::Down)), std::nullopt);
    EXPECT_EQ(form.update(pressed(MenuAction::Confirm)), std::nullopt);
    EXPECT_EQ(form.focus(), 0U);
}

TEST(ui_Form, downWrapsAround)
{
    Form form = makeForm();

    for (std::size_t expected : {SECOND_FIELD, OK_BUTTON, QUIT_BUTTON, FIRST_FIELD}) {
        EXPECT_EQ(form.update(pressed(MenuAction::Down)), std::nullopt);
        EXPECT_EQ(form.focus(), expected);
    }
}

TEST(ui_Form, upWrapsAround)
{
    Form form = makeForm();

    form.update(pressed(MenuAction::Up));
    EXPECT_EQ(form.focus(), QUIT_BUTTON);
    form.update(pressed(MenuAction::Up));
    EXPECT_EQ(form.focus(), OK_BUTTON);
}

TEST(ui_Form, leftAndRightDoNothing)
{
    Form form = makeForm();

    form.update(pressed(MenuAction::Left));
    form.update(pressed(MenuAction::Right));
    EXPECT_EQ(form.focus(), FIRST_FIELD);
}

TEST(ui_Form, confirmOnFieldMovesToNextWidget)
{
    Form form = makeForm();

    EXPECT_EQ(form.update(pressed(MenuAction::Confirm)), std::nullopt);
    EXPECT_EQ(form.focus(), SECOND_FIELD);
    EXPECT_EQ(form.update(pressed(MenuAction::Confirm)), std::nullopt);
    EXPECT_EQ(form.focus(), OK_BUTTON);
}

TEST(ui_Form, confirmOnLastFieldWraps)
{
    Form form(std::vector<RType::UI::Widget>{ Button("Ok", row(0.0F)), TextField("Field", "", 8, row(20.0F)) });

    form.update(pressed(MenuAction::Down));
    EXPECT_EQ(form.update(pressed(MenuAction::Confirm)), std::nullopt);
    EXPECT_EQ(form.focus(), 0U);
}

TEST(ui_Form, confirmOnButtonActivatesIt)
{
    Form form = makeForm();

    form.update(pressed(MenuAction::Up));
    EXPECT_EQ(form.update(pressed(MenuAction::Confirm)), activated(QUIT_BUTTON));
    EXPECT_EQ(form.focus(), QUIT_BUTTON);
}

TEST(ui_Form, stillMouseDoesNotTakeFocus)
{
    Form form = makeForm();

    form.update(idle(ON_OK_BUTTON));
    form.update(idle(ON_OK_BUTTON));
    EXPECT_EQ(form.focus(), FIRST_FIELD);
}

TEST(ui_Form, movingMouseTakesFocus)
{
    Form form = makeForm();

    form.update(idle(OUTSIDE));
    form.update(idle(ON_OK_BUTTON));
    EXPECT_EQ(form.focus(), OK_BUTTON);

    form.update(idle(ON_SECOND_FIELD));
    EXPECT_EQ(form.focus(), SECOND_FIELD);
}

TEST(ui_Form, mouseMovingOutsideKeepsFocus)
{
    Form form = makeForm();

    form.update(idle(ON_FIRST_FIELD));
    form.update(idle(ON_SECOND_FIELD));
    form.update(idle(OUTSIDE));
    EXPECT_EQ(form.focus(), SECOND_FIELD);
}

TEST(ui_Form, stillMouseDoesNotStealFocusFromKeyboard)
{
    Form form = makeForm();

    form.update(idle(ON_FIRST_FIELD));
    FrameInput input = idle(ON_FIRST_FIELD);
    input.press(MenuAction::Down);
    form.update(input);
    form.update(idle(ON_FIRST_FIELD));
    EXPECT_EQ(form.focus(), SECOND_FIELD);
}

TEST(ui_Form, clickActivatesButton)
{
    Form form = makeForm();

    form.update(idle(ON_OK_BUTTON));
    EXPECT_EQ(form.update(clicked(ON_OK_BUTTON)), activated(OK_BUTTON));
    EXPECT_EQ(form.focus(), OK_BUTTON);
}

TEST(ui_Form, clickFocusesField)
{
    Form form = makeForm();

    form.update(idle(ON_SECOND_FIELD));
    form.update(pressed(MenuAction::Up));
    EXPECT_EQ(form.update(clicked(ON_SECOND_FIELD)), std::nullopt);
    EXPECT_EQ(form.focus(), SECOND_FIELD);
}

TEST(ui_Form, clickWorksOnFirstFrame)
{
    Form form = makeForm();

    EXPECT_EQ(form.update(clicked(ON_OK_BUTTON)), activated(OK_BUTTON));
}

TEST(ui_Form, clickOutsideKeepsFocus)
{
    Form form = makeForm();

    form.update(pressed(MenuAction::Down));
    EXPECT_EQ(form.update(clicked(OUTSIDE)), std::nullopt);
    EXPECT_EQ(form.focus(), SECOND_FIELD);
}

TEST(ui_Form, textGoesToFocusedFieldOnly)
{
    Form form = makeForm();
    FrameInput input = idle();

    input.text = U"ab";
    form.update(input);
    form.update(pressed(MenuAction::Down));
    input.text = U"c";
    form.update(input);

    EXPECT_EQ(valueOf(form, FIRST_FIELD), "ab");
    EXPECT_EQ(valueOf(form, SECOND_FIELD), "c");
}

TEST(ui_Form, textOfAFrameGoesToTheFieldFocusedBeforeIt)
{
    Form form = makeForm();
    FrameInput input = pressed(MenuAction::Confirm);

    input.text = U"a";
    form.update(input);

    EXPECT_EQ(valueOf(form, FIRST_FIELD), "a");
    EXPECT_EQ(valueOf(form, SECOND_FIELD), "");
}

TEST(ui_Form, textOnButtonIsIgnored)
{
    Form form = makeForm();
    FrameInput input = idle();

    form.update(pressed(MenuAction::Up));
    input.text = U"a";
    input.backspace = true;
    EXPECT_EQ(form.update(input), std::nullopt);
    EXPECT_EQ(valueOf(form, FIRST_FIELD), "");
    EXPECT_EQ(valueOf(form, SECOND_FIELD), "");
}

TEST(ui_Form, backspaceGoesToFocusedField)
{
    Form form = makeForm();
    FrameInput input = idle();

    input.text = U"ab";
    form.update(input);

    input.text.clear();
    input.backspace = true;
    form.update(input);
    EXPECT_EQ(valueOf(form, FIRST_FIELD), "a");
}

TEST(ui_MenuControls, defaultBindings)
{
    using RType::Render::Binding;
    using RType::Render::GamepadButton;
    using RType::Render::Key;

    const auto controls = RType::UI::defaultMenuControls();

    EXPECT_EQ(controls.bindings(MenuAction::Up), (std::vector<Binding>{Key::Up, GamepadButton::DpadUp}));
    EXPECT_EQ(controls.bindings(MenuAction::Down), (std::vector<Binding>{Key::Down, Key::Tab, GamepadButton::DpadDown}));
    EXPECT_EQ(controls.bindings(MenuAction::Confirm), (std::vector<Binding>{Key::Enter, Key::KpEnter, GamepadButton::A}));
    EXPECT_EQ(controls.bindings(MenuAction::Back), (std::vector<Binding>{Key::Escape, GamepadButton::B}));
}

namespace {

    using RType::UI::FormEvent;
    using RType::UI::Slider;

    constexpr std::size_t SLIDER = 1;
    constexpr RType::Render::Rect SLIDER_BOX = { .x = 0.0F, .y = 20.0F, .width = 324.0F, .height = 10.0F };

    // Ok button, a 0-100 slider by steps of 10 at 50, Quit button
    Form makeSliderForm()
    {
        return Form(std::vector<RType::UI::Widget>{
            Button("Ok", row(0.0F)),
            Slider("Volume", 50, 0, 100, 10, SLIDER_BOX),
            Button("Quit", row(40.0F)),
        });
    }

    int sliderValue(const Form& form)
    {
        return std::get<Slider>(form.widgets()[SLIDER]).value();
    }

    float sliderX(const Form& form, float ratio)
    {
        RType::Render::Rect track = std::get<Slider>(form.widgets()[SLIDER]).track();

        return track.x + track.width * ratio;
    }

    std::optional<FormEvent> changed(std::size_t index)
    {
        return FormEvent{ .kind = FormEvent::Kind::Changed, .index = index };
    }
}


TEST(ui_Form, rightOnFocusedSliderChangesIt)
{
    Form form = makeSliderForm();

    form.update(pressed(MenuAction::Down));
    EXPECT_EQ(form.update(pressed(MenuAction::Right)), changed(SLIDER));
    EXPECT_EQ(sliderValue(form), 60);
    EXPECT_EQ(form.update(pressed(MenuAction::Left)), changed(SLIDER));
    EXPECT_EQ(sliderValue(form), 50);
}

TEST(ui_Form, sliderAtItsBoundIsNotChanged)
{
    Form form(std::vector<RType::UI::Widget>{ Slider("Volume", 100, 0, 100, 10, SLIDER_BOX) });

    EXPECT_EQ(form.update(pressed(MenuAction::Right)), std::nullopt);
}

TEST(ui_Form, leftAndRightOnAButtonLeaveTheSlider)
{
    Form form = makeSliderForm();

    EXPECT_EQ(form.update(pressed(MenuAction::Right)), std::nullopt);
    EXPECT_EQ(form.update(pressed(MenuAction::Left)), std::nullopt);
    EXPECT_EQ(sliderValue(form), 50);
    EXPECT_EQ(form.focus(), 0U);
}

TEST(ui_Form, confirmOnSliderMovesToNextWidget)
{
    Form form = makeSliderForm();

    form.update(pressed(MenuAction::Down));
    EXPECT_EQ(form.update(pressed(MenuAction::Confirm)), std::nullopt);
    EXPECT_EQ(form.focus(), SLIDER + 1);
}

TEST(ui_Form, clickOnSliderFocusesAndSetsIt)
{
    Form form = makeSliderForm();

    EXPECT_EQ(form.update(clicked({ .x = sliderX(form, 0.8F), .y = 25.0F })), changed(SLIDER));
    EXPECT_EQ(form.focus(), SLIDER);
    EXPECT_EQ(sliderValue(form), 80);
}

TEST(ui_Form, draggedSliderKeepsTheFocus)
{
    Form form = makeSliderForm();
    FrameInput input = clicked({ .x = sliderX(form, 0.5F), .y = 25.0F });

    form.update(input);

    // Held over the Quit button, then on its left: the slider still follows
    input = idle({ .x = 50.0F, .y = 45.0F });
    input.mouseDown = true;
    form.update(input);
    EXPECT_EQ(form.focus(), SLIDER);

    input.mouse = { .x = -50.0F, .y = 45.0F };
    EXPECT_EQ(form.update(input), changed(SLIDER));
    EXPECT_EQ(form.focus(), SLIDER);
    EXPECT_EQ(sliderValue(form), 0);
}

TEST(ui_Form, releasedSliderGivesTheFocusBack)
{
    Form form = makeSliderForm();

    form.update(clicked({ .x = sliderX(form, 0.5F), .y = 25.0F }));
    form.update(idle({ .x = sliderX(form, 0.5F), .y = 25.0F }));
    form.update(idle(ON_FIRST_FIELD));
    EXPECT_EQ(form.focus(), 0U);
    EXPECT_EQ(sliderValue(form), 50);
}

TEST(ui_Form, buttonsAreStillActivatedNextToASlider)
{
    Form form = makeSliderForm();

    EXPECT_EQ(form.update(pressed(MenuAction::Confirm)), activated(0));
    EXPECT_EQ(form.update(clicked({ .x = 50.0F, .y = 45.0F })), activated(SLIDER + 1));
}

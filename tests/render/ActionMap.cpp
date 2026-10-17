/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** ActionMap class
*/

#include <gtest/gtest.h>
#include <rtype/render.hpp>
#include <map>
#include <set>
#include <utility>

namespace {

    enum class Action {
        Up,
        Shoot,
        Pause,
    };

    /** Scripted input state, records the gamepad indexes it was asked for */
    struct FakeInput {
        std::set<RType::Render::Key> downKeys;
        std::set<RType::Render::Key> pressedKeys;
        std::set<RType::Render::Key> releasedKeys;
        std::set<RType::Render::MouseButton> downMouse;
        std::set<RType::Render::MouseButton> pressedMouse;
        std::set<RType::Render::MouseButton> releasedMouse;
        std::set<std::pair<int, RType::Render::GamepadButton>> downPad;
        std::set<std::pair<int, RType::Render::GamepadButton>> pressedPad;
        std::set<std::pair<int, RType::Render::GamepadButton>> releasedPad;
        std::map<std::pair<int, RType::Render::GamepadAxis>, float> axes;
        mutable std::set<int> queriedGamepads;
        mutable int axisQueries = 0;

        bool isDown(RType::Render::Key key) const { return downKeys.contains(key); }
        bool isPressed(RType::Render::Key key) const { return pressedKeys.contains(key); }
        bool isReleased(RType::Render::Key key) const { return releasedKeys.contains(key); }
        bool isDown(RType::Render::MouseButton button) const { return downMouse.contains(button); }
        bool isPressed(RType::Render::MouseButton button) const { return pressedMouse.contains(button); }
        bool isReleased(RType::Render::MouseButton button) const { return releasedMouse.contains(button); }

        bool isDown(int gamepad, RType::Render::GamepadButton button) const
        {
            queriedGamepads.insert(gamepad);
            return downPad.contains({gamepad, button});
        }

        bool isPressed(int gamepad, RType::Render::GamepadButton button) const
        {
            queriedGamepads.insert(gamepad);
            return pressedPad.contains({gamepad, button});
        }

        bool isReleased(int gamepad, RType::Render::GamepadButton button) const
        {
            queriedGamepads.insert(gamepad);
            return releasedPad.contains({gamepad, button});
        }

        float axis(int gamepad, RType::Render::GamepadAxis axis) const
        {
            queriedGamepads.insert(gamepad);
            axisQueries++;
            auto found = axes.find({gamepad, axis});

            return found == axes.end() ? 0.0F : found->second;
        }
    };

    static_assert(RType::Render::InputType<FakeInput>);

    using Map = RType::Render::ActionMap<Action>;
    using RType::Render::AxisBinding;
    using RType::Render::Binding;
    using RType::Render::GamepadAxis;
    using RType::Render::GamepadButton;
    using RType::Render::Key;
    using RType::Render::MouseButton;
}


/////////////////////////////////////////////////////////
//                                                     //
//                       BINDINGS                      //
//                                                     //
/////////////////////////////////////////////////////////

TEST(render_ActionMap, emptyByDefault)
{
    Map map;

    EXPECT_TRUE(map.bindings(Action::Up).empty());
    EXPECT_EQ(map.gamepad(), 0);
}

TEST(render_ActionMap, bindKeepsOrder)
{
    Map map;

    map.bind(Action::Up, Key::Up);
    map.bind(Action::Up, Key::W);
    map.bind(Action::Up, GamepadButton::DpadUp);
    map.bind(Action::Up, AxisBinding{GamepadAxis::LeftY, -0.5F});

    const std::vector<Binding> expected = {
        Key::Up, Key::W, GamepadButton::DpadUp, AxisBinding{GamepadAxis::LeftY, -0.5F},
    };
    EXPECT_EQ(map.bindings(Action::Up), expected);
    EXPECT_TRUE(map.bindings(Action::Shoot).empty());
}

TEST(render_ActionMap, bindDeduplicates)
{
    Map map;

    map.bind(Action::Shoot, Key::Space);
    map.bind(Action::Shoot, Key::Space);
    map.bind(Action::Shoot, MouseButton::Left);
    map.bind(Action::Shoot, MouseButton::Left);
    map.bind(Action::Shoot, AxisBinding{GamepadAxis::RightTrigger, 0.5F});
    map.bind(Action::Shoot, AxisBinding{GamepadAxis::RightTrigger, 0.5F});

    EXPECT_EQ(map.bindings(Action::Shoot).size(), 3U);
}

TEST(render_ActionMap, sameValueDifferentTypeIsNotDuplicate)
{
    Map map;

    // Key::A, MouseButton::Left and GamepadButton::DpadUp all have the value 0
    map.bind(Action::Shoot, Key::A);
    map.bind(Action::Shoot, MouseButton::Left);
    map.bind(Action::Shoot, GamepadButton::DpadUp);

    EXPECT_EQ(map.bindings(Action::Shoot).size(), 3U);
}

TEST(render_ActionMap, sameKeyOnTwoActions)
{
    Map map;

    map.bind(Action::Up, Key::Enter);
    map.bind(Action::Pause, Key::Enter);

    EXPECT_EQ(map.bindings(Action::Up).size(), 1U);
    EXPECT_EQ(map.bindings(Action::Pause).size(), 1U);
}

TEST(render_ActionMap, unbindPresent)
{
    Map map;

    map.bind(Action::Up, Key::Up);
    map.bind(Action::Up, Key::W);
    map.unbind(Action::Up, Key::Up);

    const std::vector<Binding> expected = {Key::W};
    EXPECT_EQ(map.bindings(Action::Up), expected);

    map.unbind(Action::Up, Key::W);
    EXPECT_TRUE(map.bindings(Action::Up).empty());
}

TEST(render_ActionMap, unbindAbsentIsNoop)
{
    Map map;

    map.unbind(Action::Up, Key::Up);
    EXPECT_TRUE(map.bindings(Action::Up).empty());

    map.bind(Action::Up, Key::W);
    map.unbind(Action::Up, Key::Up);
    map.unbind(Action::Up, AxisBinding{GamepadAxis::LeftY, 0.5F});
    map.unbind(Action::Shoot, Key::W);

    const std::vector<Binding> expected = {Key::W};
    EXPECT_EQ(map.bindings(Action::Up), expected);
}

TEST(render_ActionMap, unbindAxisNeedsSameThreshold)
{
    Map map;

    map.bind(Action::Up, AxisBinding{GamepadAxis::LeftY, -0.5F});
    map.unbind(Action::Up, AxisBinding{GamepadAxis::LeftY, -0.3F});
    EXPECT_EQ(map.bindings(Action::Up).size(), 1U);

    map.unbind(Action::Up, AxisBinding{GamepadAxis::LeftY, -0.5F});
    EXPECT_TRUE(map.bindings(Action::Up).empty());
}

TEST(render_ActionMap, clear)
{
    Map map;

    map.bind(Action::Up, Key::Up);
    map.bind(Action::Up, GamepadButton::DpadUp);
    map.bind(Action::Shoot, Key::Space);
    map.clear(Action::Up);
    map.clear(Action::Pause);

    EXPECT_TRUE(map.bindings(Action::Up).empty());
    EXPECT_EQ(map.bindings(Action::Shoot).size(), 1U);

    map.bind(Action::Up, Key::W);
    EXPECT_EQ(map.bindings(Action::Up).size(), 1U);
}

TEST(render_ActionMap, setGamepad)
{
    Map map;

    map.setGamepad(3);
    EXPECT_EQ(map.gamepad(), 3);
}


/////////////////////////////////////////////////////////
//                                                     //
//                       QUERIES                       //
//                                                     //
/////////////////////////////////////////////////////////

TEST(render_ActionMap, unboundActionIsNeverActive)
{
    Map map;
    FakeInput input;

    input.downKeys = {Key::Up};
    input.pressedKeys = {Key::Up};
    input.releasedKeys = {Key::Up};

    EXPECT_FALSE(map.isDown(Action::Up, input));
    EXPECT_FALSE(map.isPressed(Action::Up, input));
    EXPECT_FALSE(map.isReleased(Action::Up, input));
}

TEST(render_ActionMap, keyBinding)
{
    Map map;
    FakeInput input;

    map.bind(Action::Up, Key::Up);
    map.bind(Action::Up, Key::W);
    EXPECT_FALSE(map.isDown(Action::Up, input));

    input.downKeys = {Key::W};
    EXPECT_TRUE(map.isDown(Action::Up, input));
    EXPECT_FALSE(map.isPressed(Action::Up, input));
    EXPECT_FALSE(map.isReleased(Action::Up, input));

    input.pressedKeys = {Key::Up};
    EXPECT_TRUE(map.isPressed(Action::Up, input));

    input.releasedKeys = {Key::W};
    EXPECT_TRUE(map.isReleased(Action::Up, input));

    input.downKeys = {Key::S};
    EXPECT_FALSE(map.isDown(Action::Up, input));
}

TEST(render_ActionMap, mouseBinding)
{
    Map map;
    FakeInput input;

    map.bind(Action::Shoot, MouseButton::Left);
    input.downMouse = {MouseButton::Right};
    EXPECT_FALSE(map.isDown(Action::Shoot, input));

    input.downMouse = {MouseButton::Left};
    input.pressedMouse = {MouseButton::Left};
    input.releasedMouse = {MouseButton::Left};
    EXPECT_TRUE(map.isDown(Action::Shoot, input));
    EXPECT_TRUE(map.isPressed(Action::Shoot, input));
    EXPECT_TRUE(map.isReleased(Action::Shoot, input));
}

TEST(render_ActionMap, gamepadButtonBinding)
{
    Map map;
    FakeInput input;

    map.bind(Action::Shoot, GamepadButton::A);
    input.downPad = {{0, GamepadButton::B}};
    EXPECT_FALSE(map.isDown(Action::Shoot, input));

    input.downPad = {{0, GamepadButton::A}};
    input.pressedPad = {{0, GamepadButton::A}};
    input.releasedPad = {{0, GamepadButton::A}};
    EXPECT_TRUE(map.isDown(Action::Shoot, input));
    EXPECT_TRUE(map.isPressed(Action::Shoot, input));
    EXPECT_TRUE(map.isReleased(Action::Shoot, input));
}

TEST(render_ActionMap, setGamepadRoutesButtons)
{
    Map map;
    FakeInput input;

    map.bind(Action::Shoot, GamepadButton::A);
    input.downPad = {{2, GamepadButton::A}};
    input.pressedPad = {{2, GamepadButton::A}};
    input.releasedPad = {{2, GamepadButton::A}};
    EXPECT_FALSE(map.isDown(Action::Shoot, input));
    EXPECT_FALSE(map.isPressed(Action::Shoot, input));
    EXPECT_FALSE(map.isReleased(Action::Shoot, input));
    EXPECT_EQ(input.queriedGamepads, std::set<int>{0});

    map.setGamepad(2);
    input.queriedGamepads.clear();
    EXPECT_TRUE(map.isDown(Action::Shoot, input));
    EXPECT_TRUE(map.isPressed(Action::Shoot, input));
    EXPECT_TRUE(map.isReleased(Action::Shoot, input));
    EXPECT_EQ(input.queriedGamepads, std::set<int>{2});
}

TEST(render_ActionMap, setGamepadRoutesAxes)
{
    Map map;
    FakeInput input;

    map.bind(Action::Up, AxisBinding{GamepadAxis::LeftY, -0.5F});
    input.axes[{1, GamepadAxis::LeftY}] = -1.0F;
    EXPECT_FALSE(map.isDown(Action::Up, input));

    map.setGamepad(1);
    EXPECT_TRUE(map.isDown(Action::Up, input));
    EXPECT_EQ(input.queriedGamepads, (std::set<int>{0, 1}));
}

TEST(render_ActionMap, positiveAxisThreshold)
{
    Map map;
    FakeInput input;

    map.bind(Action::Shoot, AxisBinding{GamepadAxis::RightTrigger, 0.5F});

    input.axes[{0, GamepadAxis::RightTrigger}] = 0.49F;
    EXPECT_FALSE(map.isDown(Action::Shoot, input));
    input.axes[{0, GamepadAxis::RightTrigger}] = 0.5F;
    EXPECT_TRUE(map.isDown(Action::Shoot, input));
    input.axes[{0, GamepadAxis::RightTrigger}] = 1.0F;
    EXPECT_TRUE(map.isDown(Action::Shoot, input));
    input.axes[{0, GamepadAxis::RightTrigger}] = -1.0F;
    EXPECT_FALSE(map.isDown(Action::Shoot, input));
}

TEST(render_ActionMap, negativeAxisThreshold)
{
    Map map;
    FakeInput input;

    map.bind(Action::Up, AxisBinding{GamepadAxis::LeftY, -0.5F});

    input.axes[{0, GamepadAxis::LeftY}] = -0.49F;
    EXPECT_FALSE(map.isDown(Action::Up, input));
    input.axes[{0, GamepadAxis::LeftY}] = -0.5F;
    EXPECT_TRUE(map.isDown(Action::Up, input));
    input.axes[{0, GamepadAxis::LeftY}] = -1.0F;
    EXPECT_TRUE(map.isDown(Action::Up, input));
    input.axes[{0, GamepadAxis::LeftY}] = 1.0F;
    EXPECT_FALSE(map.isDown(Action::Up, input));
}

TEST(render_ActionMap, zeroAxisThresholdIsNeverActive)
{
    Map map;
    FakeInput input;

    map.bind(Action::Up, AxisBinding{GamepadAxis::LeftX, 0.0F});

    for (float value : {-1.0F, -0.5F, 0.0F, 0.5F, 1.0F}) {
        input.axes[{0, GamepadAxis::LeftX}] = value;
        EXPECT_FALSE(map.isDown(Action::Up, input)) << "axis value " << value;
    }
}

TEST(render_ActionMap, axisIgnoredByPressedAndReleased)
{
    Map map;
    FakeInput input;

    map.bind(Action::Up, AxisBinding{GamepadAxis::LeftY, -0.5F});
    input.axes[{0, GamepadAxis::LeftY}] = -1.0F;

    EXPECT_TRUE(map.isDown(Action::Up, input));
    input.axisQueries = 0;
    EXPECT_FALSE(map.isPressed(Action::Up, input));
    EXPECT_FALSE(map.isReleased(Action::Up, input));
    EXPECT_EQ(input.axisQueries, 0);
}

TEST(render_ActionMap, anyBindingActivates)
{
    Map map;
    FakeInput input;

    map.bind(Action::Up, Key::Up);
    map.bind(Action::Up, GamepadButton::DpadUp);
    map.bind(Action::Up, AxisBinding{GamepadAxis::LeftY, -0.5F});

    input.axes[{0, GamepadAxis::LeftY}] = -0.8F;
    EXPECT_TRUE(map.isDown(Action::Up, input));

    input.axes.clear();
    input.downPad = {{0, GamepadButton::DpadUp}};
    EXPECT_TRUE(map.isDown(Action::Up, input));

    input.downPad.clear();
    EXPECT_FALSE(map.isDown(Action::Up, input));
}

TEST(render_ActionMap, unbindStopsActivation)
{
    Map map;
    FakeInput input;

    map.bind(Action::Up, Key::Up);
    input.downKeys = {Key::Up};
    EXPECT_TRUE(map.isDown(Action::Up, input));

    map.unbind(Action::Up, Key::Up);
    EXPECT_FALSE(map.isDown(Action::Up, input));
}

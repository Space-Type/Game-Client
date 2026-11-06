/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** player controls
*/

#include "Controls.hpp"
#include <algorithm>
#include <type_traits>
#include <variant>


auto RType::Game::defaultControls() -> Controls
{
    using Render::AxisBinding;
    using Render::GamepadAxis;
    using Render::GamepadButton;
    using Render::Key;

    Controls controls;

    controls.bind(Action::Up, Key::Up);
    controls.bind(Action::Up, GamepadButton::DpadUp);
    controls.bind(Action::Up, AxisBinding{ GamepadAxis::LeftY, -0.5f });
    controls.bind(Action::Down, Key::Down);
    controls.bind(Action::Down, GamepadButton::DpadDown);
    controls.bind(Action::Down, AxisBinding{ GamepadAxis::LeftY, 0.5f });
    controls.bind(Action::Left, Key::Left);
    controls.bind(Action::Left, GamepadButton::DpadLeft);
    controls.bind(Action::Left, AxisBinding{ GamepadAxis::LeftX, -0.5f });
    controls.bind(Action::Right, Key::Right);
    controls.bind(Action::Right, GamepadButton::DpadRight);
    controls.bind(Action::Right, AxisBinding{ GamepadAxis::LeftX, 0.5f });
    controls.bind(Action::Shoot, Key::Space);
    controls.bind(Action::Shoot, GamepadButton::A);

    return controls;
}

std::string_view RType::Game::actionName(Action action) noexcept
{
    switch (action) {
        case Action::Up: return "Up";
        case Action::Down: return "Down";
        case Action::Left: return "Left";
        case Action::Right: return "Right";
        case Action::Shoot: return "Shoot";
    }
    return "Unknown";
}

std::string_view RType::Game::bindingName(const Render::Binding& binding) noexcept
{
    return std::visit([](const auto& value) -> std::string_view {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, Render::Key>)
            return Render::keyName(value);
        else if constexpr (std::is_same_v<T, Render::MouseButton>)
            return Render::mouseButtonName(value);
        else if constexpr (std::is_same_v<T, Render::GamepadButton>)
            return Render::gamepadButtonName(value);
        else
            return "Axis";
    }, binding);
}

bool RType::Game::fitsSlot(const Render::Binding& binding, Slot slot) noexcept
{
    if (slot == Slot::Gamepad)
        return std::holds_alternative<Render::GamepadButton>(binding);
    return std::holds_alternative<Render::Key>(binding) || std::holds_alternative<Render::MouseButton>(binding);
}

auto RType::Game::slotBinding(const Controls& controls, Action action, Slot slot) -> std::optional<Render::Binding>
{
    const std::vector<Render::Binding>& bindings = controls.bindings(action);
    auto it = std::ranges::find_if(bindings, [&](const Render::Binding& binding) { return fitsSlot(binding, slot); });

    if (it == bindings.end())
        return std::nullopt;
    return *it;
}

void RType::Game::rebind(Controls& controls, Action action, Slot slot, Render::Binding binding)
{
    if (!fitsSlot(binding, slot))
        return;
    if (std::optional<Render::Binding> old = slotBinding(controls, action, slot))
        controls.unbind(action, *old);
    controls.bind(action, binding);
}

bool RType::Game::conflicts(const Controls& controls, Action action)
{
    return std::ranges::any_of(controls.bindings(action), [&](const Render::Binding& binding) {
        if (!fitsSlot(binding, Slot::Keyboard) && !fitsSlot(binding, Slot::Gamepad))
            return false;
        return std::ranges::any_of(ACTIONS, [&](Action other) {
            return other != action && std::ranges::find(controls.bindings(other), binding) != controls.bindings(other).end();
        });
    });
}

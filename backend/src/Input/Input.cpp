/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Source file for
** Input class
*/

#include <backend/Input.hpp>
#include <backend/Window.hpp>
#include "Input/KeyMap.hpp"

namespace {

    /** True if the enum value is a real input (below `Count`) */
    template <typename E>
    constexpr bool isValid(E value) noexcept
    {
        return Backend::Detail::indexOf(value) < Backend::Detail::COUNT_OF<E>;
    }

    /** True if both the gamepad index and the enum value are usable */
    template <typename E>
    constexpr bool isValid(int gamepad, E value) noexcept
    {
        return gamepad >= 0 && isValid(value);
    }
}

Backend::Input::Input(const Window& window) noexcept
    : m_Window(window)
{
}


/////////////////////////////////////////////////////////
//                                                     //
//                       KEYBOARD                      //
//                                                     //
/////////////////////////////////////////////////////////

bool Backend::Input::isDown(Key key) const noexcept
{
    return isValid(key) && IsKeyDown(Detail::toRaylib(key));
}

bool Backend::Input::isPressed(Key key) const noexcept
{
    return isValid(key) && IsKeyPressed(Detail::toRaylib(key));
}

bool Backend::Input::isReleased(Key key) const noexcept
{
    return isValid(key) && IsKeyReleased(Detail::toRaylib(key));
}

std::optional<Backend::Key> Backend::Input::nextPressed() noexcept
{
    for (int key = GetKeyPressed(); key != KEY_NULL; key = GetKeyPressed()) {
        std::optional<Key> mapped = Detail::fromRaylibKey(key);

        if (mapped)
            return mapped;
    }
    return std::nullopt;
}


/////////////////////////////////////////////////////////
//                                                     //
//                         MOUSE                       //
//                                                     //
/////////////////////////////////////////////////////////

bool Backend::Input::isDown(MouseButton button) const noexcept
{
    return isValid(button) && IsMouseButtonDown(Detail::toRaylib(button));
}

bool Backend::Input::isPressed(MouseButton button) const noexcept
{
    return isValid(button) && IsMouseButtonPressed(Detail::toRaylib(button));
}

bool Backend::Input::isReleased(MouseButton button) const noexcept
{
    return isValid(button) && IsMouseButtonReleased(Detail::toRaylib(button));
}

Backend::Vec2 Backend::Input::mousePosition() const noexcept
{
    ::Vector2 position = GetMousePosition();

    return m_Window.screenToVirtual({position.x, position.y});
}

float Backend::Input::mouseWheel() const noexcept
{
    return GetMouseWheelMove();
}


/////////////////////////////////////////////////////////
//                                                     //
//                        GAMEPAD                      //
//                                                     //
/////////////////////////////////////////////////////////

bool Backend::Input::isGamepadAvailable(int gamepad) const noexcept
{
    return gamepad >= 0 && IsGamepadAvailable(gamepad);
}

bool Backend::Input::isDown(int gamepad, GamepadButton button) const noexcept
{
    return isValid(gamepad, button) && IsGamepadButtonDown(gamepad, Detail::toRaylib(button));
}

bool Backend::Input::isPressed(int gamepad, GamepadButton button) const noexcept
{
    return isValid(gamepad, button) && IsGamepadButtonPressed(gamepad, Detail::toRaylib(button));
}

bool Backend::Input::isReleased(int gamepad, GamepadButton button) const noexcept
{
    return isValid(gamepad, button) && IsGamepadButtonReleased(gamepad, Detail::toRaylib(button));
}

float Backend::Input::axis(int gamepad, GamepadAxis axis) const noexcept
{
    if (!isValid(gamepad, axis) || !IsGamepadAvailable(gamepad))
        return 0.0F;
    return Detail::normalizeAxis(axis, GetGamepadAxisMovement(gamepad, Detail::toRaylib(axis)));
}

/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Source file for
** Input class
*/

#include "Input.hpp"
#include "../Window/Window.hpp"
#include "KeyMap.hpp"

namespace {

    /** True if the enum value is a real input (below `Count`) */
    template <typename E>
    constexpr bool isValid(E value) noexcept
    {
        return RType::Render::Detail::indexOf(value) < RType::Render::Detail::COUNT_OF<E>;
    }

    /** True if both the gamepad index and the enum value are usable */
    template <typename E>
    constexpr bool isValid(int gamepad, E value) noexcept
    {
        return gamepad >= 0 && isValid(value);
    }
}

RType::Render::Input::Input(const Window& window) noexcept
    : m_Window(window)
{
}


/////////////////////////////////////////////////////////
//                                                     //
//                       KEYBOARD                      //
//                                                     //
/////////////////////////////////////////////////////////

bool RType::Render::Input::isDown(Key key) const noexcept
{
    return isValid(key) && IsKeyDown(Detail::toRaylib(key));
}

bool RType::Render::Input::isPressed(Key key) const noexcept
{
    return isValid(key) && IsKeyPressed(Detail::toRaylib(key));
}

bool RType::Render::Input::isReleased(Key key) const noexcept
{
    return isValid(key) && IsKeyReleased(Detail::toRaylib(key));
}

std::optional<RType::Render::Key> RType::Render::Input::nextPressed() noexcept
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

bool RType::Render::Input::isDown(MouseButton button) const noexcept
{
    return isValid(button) && IsMouseButtonDown(Detail::toRaylib(button));
}

bool RType::Render::Input::isPressed(MouseButton button) const noexcept
{
    return isValid(button) && IsMouseButtonPressed(Detail::toRaylib(button));
}

bool RType::Render::Input::isReleased(MouseButton button) const noexcept
{
    return isValid(button) && IsMouseButtonReleased(Detail::toRaylib(button));
}

RType::Render::Vec2 RType::Render::Input::mousePosition() const noexcept
{
    ::Vector2 position = GetMousePosition();

    return m_Window.screenToVirtual({position.x, position.y});
}

float RType::Render::Input::mouseWheel() const noexcept
{
    return GetMouseWheelMove();
}


/////////////////////////////////////////////////////////
//                                                     //
//                        GAMEPAD                      //
//                                                     //
/////////////////////////////////////////////////////////

bool RType::Render::Input::isGamepadAvailable(int gamepad) const noexcept
{
    return gamepad >= 0 && IsGamepadAvailable(gamepad);
}

bool RType::Render::Input::isDown(int gamepad, GamepadButton button) const noexcept
{
    return isValid(gamepad, button) && IsGamepadButtonDown(gamepad, Detail::toRaylib(button));
}

bool RType::Render::Input::isPressed(int gamepad, GamepadButton button) const noexcept
{
    return isValid(gamepad, button) && IsGamepadButtonPressed(gamepad, Detail::toRaylib(button));
}

bool RType::Render::Input::isReleased(int gamepad, GamepadButton button) const noexcept
{
    return isValid(gamepad, button) && IsGamepadButtonReleased(gamepad, Detail::toRaylib(button));
}

float RType::Render::Input::axis(int gamepad, GamepadAxis axis) const noexcept
{
    if (!isValid(gamepad, axis) || !IsGamepadAvailable(gamepad))
        return 0.0F;
    return Detail::normalizeAxis(axis, GetGamepadAxisMovement(gamepad, Detail::toRaylib(axis)));
}

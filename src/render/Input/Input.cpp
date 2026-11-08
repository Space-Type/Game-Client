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
#define GLFW_INCLUDE_NONE  // no OpenGL header: only the joystick API is used
#include <GLFW/glfw3.h>
#include <fstream>
#include <iterator>
#include <system_error>

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

std::optional<char32_t> RType::Render::Input::nextChar() noexcept
{
    int character = GetCharPressed();

    if (character <= 0)
        return std::nullopt;
    return static_cast<char32_t>(character);
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

RType::Render::Vec2D RType::Render::Input::mousePosition() const noexcept
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

bool RType::Render::Input::addGamepadMappings(const std::string& mappings) noexcept
{
    return !mappings.empty() && SetGamepadMappings(mappings.c_str()) == GLFW_TRUE;
}

bool RType::Render::Input::loadGamepadMappings(const std::filesystem::path& file)
{
    std::error_code error;

    if (!std::filesystem::is_regular_file(file, error) || std::filesystem::file_size(file, error) > MAPPINGS_FILE_MAX || error)
        return false;

    std::ifstream stream(file, std::ios::binary);
    std::string text((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

    return stream.good() || stream.eof() ? addGamepadMappings(text) : false;
}

// raylib counts any joystick as available, mapped or not: GLFW tells them apart
bool RType::Render::Input::isGamepadAvailable(int gamepad) const noexcept
{
    return gamepad >= 0 && gamepad < MAX_GAMEPADS && IsGamepadAvailable(gamepad) && glfwJoystickIsGamepad(gamepad) == GLFW_TRUE;
}

bool RType::Render::Input::isUnmappedJoystick(int gamepad) const noexcept
{
    return gamepad >= 0 && gamepad < MAX_GAMEPADS && glfwJoystickPresent(gamepad) == GLFW_TRUE && glfwJoystickIsGamepad(gamepad) != GLFW_TRUE;
}

std::string RType::Render::Input::gamepadName(int gamepad) const
{
    if (gamepad < 0 || gamepad >= MAX_GAMEPADS || glfwJoystickPresent(gamepad) != GLFW_TRUE)
        return {};

    const char* name = glfwGetJoystickName(gamepad);

    return name ? name : "";
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
    if (!isValid(gamepad, axis) || !isGamepadAvailable(gamepad))
        return 0.0F;
    return Detail::normalizeAxis(axis, GetGamepadAxisMovement(gamepad, Detail::toRaylib(axis)));
}

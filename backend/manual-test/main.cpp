/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Manual test program
** for the backend library
*/

#include <backend.hpp>
#include <chrono>
#include <exception>
#include <iostream>
#include <string>

// Manual check of everything that needs a real window
// or audio device (not covered by the unit tests).
//
// Usage: backend_manual_test <texture> [sound] [music] [font]
//
// Arrows / gamepad: move   Space: sound   M: music
// +/-: master volume       F11: fullscreen   Escape: quit

namespace {

    enum class Action {
        Up,
        Down,
        Left,
        Right,
        Shoot,
    };

    constexpr float SPEED = 600.0F;          // Virtual pixels per second
    constexpr float SCROLL_SPEED = 60.0F;    // Background texture pixels per second

    Backend::ActionMap<Action> makeControls()
    {
        Backend::ActionMap<Action> controls;

        controls.bind(Action::Up, Backend::Key::Up);
        controls.bind(Action::Up, Backend::GamepadButton::DpadUp);
        controls.bind(Action::Up, Backend::AxisBinding{Backend::GamepadAxis::LeftY, -0.5F});
        controls.bind(Action::Down, Backend::Key::Down);
        controls.bind(Action::Down, Backend::GamepadButton::DpadDown);
        controls.bind(Action::Down, Backend::AxisBinding{Backend::GamepadAxis::LeftY, 0.5F});
        controls.bind(Action::Left, Backend::Key::Left);
        controls.bind(Action::Left, Backend::GamepadButton::DpadLeft);
        controls.bind(Action::Left, Backend::AxisBinding{Backend::GamepadAxis::LeftX, -0.5F});
        controls.bind(Action::Right, Backend::Key::Right);
        controls.bind(Action::Right, Backend::GamepadButton::DpadRight);
        controls.bind(Action::Right, Backend::AxisBinding{Backend::GamepadAxis::LeftX, 0.5F});
        controls.bind(Action::Shoot, Backend::Key::Space);
        controls.bind(Action::Shoot, Backend::GamepadButton::A);
        return controls;
    }

    int run(int argc, char* argv[])
    {
        Backend::Window window({.title = "Backend manual test"});
        Backend::Resources resources(window);
        Backend::Renderer renderer(resources);
        Backend::Input input(window);
        Backend::Audio audio;
        Backend::ActionMap<Action> controls = makeControls();

        Backend::TextureId texture = resources.loadTexture(argv[1]);
        Backend::SoundId sound = argc > 2 ? audio.loadSound(argv[2]) : Backend::SoundId::Invalid;
        Backend::MusicId music = argc > 3 ? audio.loadMusic(argv[3]) : Backend::MusicId::Invalid;
        Backend::FontId font = argc > 4 ? resources.loadFont(argv[4], 48) : Backend::FontId::Default;

        Backend::Vec2 virtualSize = window.virtualSize();
        Backend::Vec2 position = virtualSize * 0.5F;
        Backend::Vec2 scroll = {};
        bool musicPlaying = false;
        std::string lastKey = "none";

        auto previous = std::chrono::steady_clock::now();
        while (!window.shouldClose()) {
            auto now = std::chrono::steady_clock::now();
            float delta = std::chrono::duration<float>(now - previous).count();
            previous = now;

            if (input.isPressed(Backend::Key::Escape))
                window.close();
            if (input.isPressed(Backend::Key::F11))
                window.toggleFullscreen();
            if (controls.isPressed(Action::Shoot, input))
                audio.play(sound);
            if (input.isPressed(Backend::Key::M)) {
                musicPlaying ? audio.stopMusic() : audio.playMusic(music);
                musicPlaying = !musicPlaying;
            }
            if (input.isPressed(Backend::Key::KpAdd) || input.isPressed(Backend::Key::Equal))
                audio.setVolume(Backend::Channel::Master, audio.volume(Backend::Channel::Master) + 0.1F);
            if (input.isPressed(Backend::Key::KpSubtract) || input.isPressed(Backend::Key::Minus))
                audio.setVolume(Backend::Channel::Master, audio.volume(Backend::Channel::Master) - 0.1F);
            for (auto key = input.nextPressed(); key; key = input.nextPressed())
                lastKey = std::string(Backend::keyName(*key));

            Backend::Vec2 direction = {
                (controls.isDown(Action::Right, input) ? 1.0F : 0.0F) - (controls.isDown(Action::Left, input) ? 1.0F : 0.0F),
                (controls.isDown(Action::Down, input) ? 1.0F : 0.0F) - (controls.isDown(Action::Up, input) ? 1.0F : 0.0F),
            };
            position = position + direction * (SPEED * delta);
            scroll.x += SCROLL_SPEED * delta;
            audio.update();

            window.beginFrame(Backend::Colors::BLACK);
            renderer.drawScrolling(texture, {0.0F, 0.0F, virtualSize.x, virtualSize.y}, scroll, 2.0F,
                                   Backend::Color{255, 255, 255, 90});
            renderer.drawSprite(texture, {.position = position, .origin = {0.5F, 0.5F}, .scale = {2.0F, 2.0F}});
            renderer.drawRectLines({0.0F, 0.0F, virtualSize.x, virtualSize.y}, 4.0F, Backend::Colors::RED);
            renderer.drawCircle(input.mousePosition(), 12.0F, Backend::Colors::YELLOW);

            std::string status = "fps " + std::to_string(delta > 0.0F ? static_cast<int>(1.0F / delta) : 0)
                + "  volume " + std::to_string(static_cast<int>(audio.volume(Backend::Channel::Master) * 100.0F))
                + "%  audio " + (audio.isReady() ? "on" : "off") + "  last key " + lastKey;
            Backend::Vec2 size = renderer.measureText(status, 40.0F, font);
            renderer.drawRect({16.0F, 16.0F, size.x + 16.0F, size.y + 16.0F}, Backend::Color{0, 0, 0, 160});
            renderer.drawText(status, {24.0F, 24.0F}, 40.0F, Backend::Colors::WHITE, font);
            window.endFrame(Backend::Colors::GRAY);
        }
        return 0;
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <texture> [sound] [music] [font]" << std::endl;
        return 84;
    }
    try {
        return run(argc, argv);
    } catch (const std::exception& error) {
        std::cerr << "backend_manual_test: " << error.what() << std::endl;
        return 84;
    }
}

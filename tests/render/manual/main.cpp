/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Manual test program
** for the render library
*/

#include <rtype/render.hpp>
#include <chrono>
#include <exception>
#include <iostream>
#include <string>

// Manual check of everything that needs a real window
// or audio device (not covered by the unit tests).
//
// Usage: render_manual_test <texture> [sound] [music] [font]
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

    RType::Render::ActionMap<Action> makeControls()
    {
        RType::Render::ActionMap<Action> controls;

        controls.bind(Action::Up, RType::Render::Key::Up);
        controls.bind(Action::Up, RType::Render::GamepadButton::DpadUp);
        controls.bind(Action::Up, RType::Render::AxisBinding{RType::Render::GamepadAxis::LeftY, -0.5F});
        controls.bind(Action::Down, RType::Render::Key::Down);
        controls.bind(Action::Down, RType::Render::GamepadButton::DpadDown);
        controls.bind(Action::Down, RType::Render::AxisBinding{RType::Render::GamepadAxis::LeftY, 0.5F});
        controls.bind(Action::Left, RType::Render::Key::Left);
        controls.bind(Action::Left, RType::Render::GamepadButton::DpadLeft);
        controls.bind(Action::Left, RType::Render::AxisBinding{RType::Render::GamepadAxis::LeftX, -0.5F});
        controls.bind(Action::Right, RType::Render::Key::Right);
        controls.bind(Action::Right, RType::Render::GamepadButton::DpadRight);
        controls.bind(Action::Right, RType::Render::AxisBinding{RType::Render::GamepadAxis::LeftX, 0.5F});
        controls.bind(Action::Shoot, RType::Render::Key::Space);
        controls.bind(Action::Shoot, RType::Render::GamepadButton::A);
        return controls;
    }

    int run(int argc, char* argv[])
    {
        RType::Render::Window window({.title = "Render manual test"});
        RType::Render::Resources resources(window);
        RType::Render::Renderer renderer(resources);
        RType::Render::Input input(window);
        RType::Render::Audio audio;
        RType::Render::ActionMap<Action> controls = makeControls();

        RType::Render::TextureId texture = resources.loadTexture(argv[1]);
        RType::Render::SoundId sound = argc > 2 ? audio.loadSound(argv[2]) : RType::Render::SoundId::Invalid;
        RType::Render::MusicId music = argc > 3 ? audio.loadMusic(argv[3]) : RType::Render::MusicId::Invalid;
        RType::Render::FontId font = argc > 4 ? resources.loadFont(argv[4], 48) : RType::Render::FontId::Default;

        RType::Render::Vec2D virtualSize = window.virtualSize();
        RType::Render::Vec2D position = virtualSize * 0.5F;
        RType::Render::Vec2D scroll = {};
        bool musicPlaying = false;
        std::string lastKey = "none";

        auto previous = std::chrono::steady_clock::now();
        while (!window.shouldClose()) {
            auto now = std::chrono::steady_clock::now();
            float delta = std::chrono::duration<float>(now - previous).count();
            previous = now;

            if (input.isPressed(RType::Render::Key::Escape))
                window.close();
            if (input.isPressed(RType::Render::Key::F11))
                window.toggleFullscreen();
            if (controls.isPressed(Action::Shoot, input))
                audio.play(sound);
            if (input.isPressed(RType::Render::Key::M)) {
                musicPlaying ? audio.stopMusic() : audio.playMusic(music);
                musicPlaying = !musicPlaying;
            }
            if (input.isPressed(RType::Render::Key::KpAdd) || input.isPressed(RType::Render::Key::Equal))
                audio.setVolume(RType::Render::Channel::Master, audio.volume(RType::Render::Channel::Master) + 0.1F);
            if (input.isPressed(RType::Render::Key::KpSubtract) || input.isPressed(RType::Render::Key::Minus))
                audio.setVolume(RType::Render::Channel::Master, audio.volume(RType::Render::Channel::Master) - 0.1F);
            for (auto key = input.nextPressed(); key; key = input.nextPressed())
                lastKey = std::string(RType::Render::keyName(*key));

            RType::Render::Vec2D direction = {
                (controls.isDown(Action::Right, input) ? 1.0F : 0.0F) - (controls.isDown(Action::Left, input) ? 1.0F : 0.0F),
                (controls.isDown(Action::Down, input) ? 1.0F : 0.0F) - (controls.isDown(Action::Up, input) ? 1.0F : 0.0F),
            };
            position = position + direction * (SPEED * delta);
            scroll.x += SCROLL_SPEED * delta;
            audio.update();

            window.beginFrame(RType::Render::Colors::BLACK);
            renderer.drawScrolling(texture, {0.0F, 0.0F, virtualSize.x, virtualSize.y}, scroll, 2.0F,
                                   RType::Render::Color{255, 255, 255, 90});
            renderer.drawSprite(texture, {.position = position, .origin = {0.5F, 0.5F}, .scale = {2.0F, 2.0F}});
            renderer.drawRectLines({0.0F, 0.0F, virtualSize.x, virtualSize.y}, 4.0F, RType::Render::Colors::RED);
            renderer.drawCircle(input.mousePosition(), 12.0F, RType::Render::Colors::YELLOW);

            std::string status = "fps " + std::to_string(delta > 0.0F ? static_cast<int>(1.0F / delta) : 0)
                + "  volume " + std::to_string(static_cast<int>(audio.volume(RType::Render::Channel::Master) * 100.0F))
                + "%  audio " + (audio.isReady() ? "on" : "off") + "  last key " + lastKey;
            RType::Render::Vec2D size = renderer.measureText(status, 40.0F, font);
            renderer.drawRect({16.0F, 16.0F, size.x + 16.0F, size.y + 16.0F}, RType::Render::Color{0, 0, 0, 160});
            renderer.drawText(status, {24.0F, 24.0F}, 40.0F, RType::Render::Colors::WHITE, font);
            window.endFrame(RType::Render::Colors::GRAY);
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
        std::cerr << "render_manual_test: " << error.what() << std::endl;
        return 84;
    }
}

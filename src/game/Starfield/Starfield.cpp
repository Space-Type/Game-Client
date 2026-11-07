/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Starfield class
*/

#include "Starfield.hpp"
#include <array>
#include <chrono>
#include <cmath>
#include <random>


namespace {
    struct Layer {
        std::size_t count;
        float size;
        float speed;
        std::uint8_t brightness;
    };

    // Far to near: more stars far away for depth, slow enough to stay in the background
    constexpr std::array LAYERS = {
        Layer{ .count = 70, .size = 1.0F, .speed = 12.0F, .brightness = 110 },
        Layer{ .count = 35, .size = 2.0F, .speed = 30.0F, .brightness = 170 },
        Layer{ .count = 15, .size = 3.0F, .speed = 60.0F, .brightness = 235 },
    };
}


RType::Game::Starfield::Starfield(Render::Vec2D size, std::uint32_t seed)
    : m_Size(size)
    , m_Stars()
    , m_Random(seed)
{
    std::uniform_real_distribution<float> x(0.0F, size.x);
    std::uniform_real_distribution<float> y(0.0F, size.y);

    for (const Layer& layer : LAYERS)
        for (std::size_t i = 0; i < layer.count; ++i)
            m_Stars.push_back(Star{
                .position = { .x = x(m_Random), .y = y(m_Random) },
                .size = layer.size,
                .speed = layer.speed,
                .color = { .r = layer.brightness, .g = layer.brightness, .b = 255, .a = 255 },
            });
}

/**
 * A star leaving on the left comes back on the right at a
 * random height: x stays in [0, width), whatever the time step.
 * Only a few stars wrap each second, so the random draws cost nothing.
 */
void RType::Game::Starfield::update(Wrecks::DeltaTime dt) noexcept
{
    float seconds = std::chrono::duration<float>(dt).count();
    std::uniform_real_distribution<float> height(0.0F, m_Size.y);

    for (Star& star : m_Stars) {
        float x = star.position.x - star.speed * seconds;

        if (x >= 0.0F) {
            star.position.x = x;
            continue;
        }
        star.position.x = std::fmod(x, m_Size.x);
        if (star.position.x < 0.0F)
            star.position.x += m_Size.x;
        star.position.y = height(m_Random);
    }
}

void RType::Game::Starfield::draw(const Render::Renderer& renderer) const
{
    for (const Star& star : m_Stars)
        renderer.drawRect({ .x = star.position.x, .y = star.position.y, .width = star.size, .height = star.size }, star.color);
}

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
        float size;
        float speed;
        std::uint8_t brightness;
    };

    // Far to near: slow enough to stay in the background
    constexpr std::array LAYERS = {
        Layer{ .size = 1.0F, .speed = 12.0F, .brightness = 110 },
        Layer{ .size = 2.0F, .speed = 30.0F, .brightness = 170 },
        Layer{ .size = 3.0F, .speed = 60.0F, .brightness = 235 },
    };
}


RType::Game::Starfield::Starfield(Render::Vec2D size, std::uint32_t seed)
    : m_Size(size)
    , m_Stars()
{
    std::minstd_rand random(seed);
    std::uniform_real_distribution<float> x(0.0F, size.x);
    std::uniform_real_distribution<float> y(0.0F, size.y);

    m_Stars.reserve(LAYERS.size() * STARS_PER_LAYER);
    for (const Layer& layer : LAYERS)
        for (std::size_t i = 0; i < STARS_PER_LAYER; ++i)
            m_Stars.push_back(Star{
                .position = { .x = x(random), .y = y(random) },
                .size = layer.size,
                .speed = layer.speed,
                .color = { .r = layer.brightness, .g = layer.brightness, .b = 255, .a = 255 },
            });
}

/**
 * A star leaving on the left comes back on the right:
 * x stays in [0, width), whatever the time step.
 */
void RType::Game::Starfield::update(Wrecks::DeltaTime dt) noexcept
{
    float seconds = std::chrono::duration<float>(dt).count();

    for (Star& star : m_Stars) {
        star.position.x = std::fmod(star.position.x - star.speed * seconds, m_Size.x);
        if (star.position.x < 0.0F)
            star.position.x += m_Size.x;
    }
}

void RType::Game::Starfield::draw(const Render::Renderer& renderer) const
{
    for (const Star& star : m_Stars)
        renderer.drawRect({ .x = star.position.x, .y = star.position.y, .width = star.size, .height = star.size }, star.color);
}

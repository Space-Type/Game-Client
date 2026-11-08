/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** starfield entities
*/

#include "Starfield.hpp"
#include <array>
#include <cstddef>
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


void RType::Game::spawnStarfield(Engine& engine, Render::Vec2D area, std::uint32_t seed)
{
    std::minstd_rand random(seed);
    std::uniform_real_distribution<float> x(0.0F, area.x);
    std::uniform_real_distribution<float> y(0.0F, area.y);

    for (const Layer& layer : LAYERS) {
        for (std::size_t i = 0; i < layer.count; ++i) {
            auto star = engine.createEntity();

            engine.addComponent(star, Components::Star{
                .position = { .x = x(random), .y = y(random) },
                .size = layer.size,
                .speed = layer.speed,
                .color = { .r = layer.brightness, .g = layer.brightness, .b = 255, .a = 255 },
            });
        }
    }
}

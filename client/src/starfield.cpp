#include "../include/starfield.hpp"

#include <cstdlib>

namespace client {

static float randomFloat(float min, float max)
{
    return min + (max - min) * (std::rand() / static_cast<float>(RAND_MAX));
}

Starfield::Starfield(float width, float height, int count)
    : width_(width), height_(height)
{
    for (int i = 0; i < count; ++i) {
        stars_.push_back({
            randomFloat(0.f, width_),  //position du dot pour la largeur
            randomFloat(0.f, height_), //position du dot pour la hauteur
            randomFloat(50.f, 200.f),  //vitesse du dot
            randomFloat(1.f, 4.f)      //taille du dot
        });
    }
}

void Starfield::update(float dt)
{
    for (auto& star : stars_) {
        star.x -= star.speed * dt;
        if (star.x < 0.f) {
            star.x = width_;
            star.y = randomFloat(0.f, height_);
        }
    }
}

void Starfield::draw(sf::RenderWindow& window)
{
    for (const auto& star : stars_) {
        sf::RectangleShape dot(sf::Vector2f(star.size, star.size));
        dot.setPosition(star.x, star.y);
        dot.setFillColor(sf::Color(200, 200, 200));
        window.draw(dot);
    }
}

}
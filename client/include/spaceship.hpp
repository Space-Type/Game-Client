#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace client {

class Spaceship {
public:
    Spaceship(float width, float height);
    Spaceship(float width, float height, const std::string& texturePath);
    ~Spaceship() = default;

    void setTexture(const std::string& texturePath);
    void update(float dt);
    void draw(sf::RenderWindow& window);

private:
    float width_;
    float height_;
    float x_;
    float y_;
    float speed_;
    sf::RectangleShape shape_;
    sf::Sprite sprite_;
    sf::Texture texture_;
    bool hasTexture_;
};

}
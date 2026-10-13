#include "../include/spaceship.hpp"

namespace client {

Spaceship::Spaceship(float width, float height)
    : width_(width), height_(height), x_(width_ * 0.15f), y_(height_ * 0.5f),
      speed_(320.f), hasTexture_(false)
{
    shape_.setSize(sf::Vector2f(28.f, 18.f));
    shape_.setFillColor(sf::Color::White);
    shape_.setPosition(x_, y_);
    shape_.setOrigin(14.f, 9.f);
}

Spaceship::Spaceship(float width, float height, const std::string& texturePath)
    : Spaceship(width, height)
{
    setTexture(texturePath);
}

void Spaceship::setTexture(const std::string& texturePath)
{
    if (!texture_.loadFromFile(texturePath)) {
        hasTexture_ = false;
        return;
    }

    sprite_.setTexture(texture_);
    sprite_.setPosition(x_, y_);
    sprite_.setOrigin(texture_.getSize().x / 2.f, texture_.getSize().y / 2.f);
    sprite_.setScale(0.5f, 0.5f);
    hasTexture_ = true;
}

void Spaceship::update(float dt)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        y_ -= speed_ * dt;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        y_ += speed_ * dt;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        x_ -= speed_ * dt;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        x_ += speed_ * dt;
    }

    if (x_ < 0.f) x_ = 0.f;
    if (x_ > width_) x_ = width_;
    if (y_ < 0.f) y_ = 0.f;
    if (y_ > height_) y_ = height_;

    shape_.setPosition(x_, y_);
    sprite_.setPosition(x_, y_);
}

void Spaceship::draw(sf::RenderWindow& window)
{
    if (hasTexture_) {
        window.draw(sprite_);
    } else {
        window.draw(shape_);
    }
}

}
#include "../include/starfield.hpp"
#include "../include/spaceship.hpp"

#include <SFML/Graphics.hpp>

int main()
{
    const float width = 1280.f;
    const float height = 720.f;

    sf::RenderWindow window(sf::VideoMode(1280, 720), "R-Type");
    client::Starfield starfield(width, height, 100);
    client::Spaceship spaceship(width, height);
    spaceship.setTexture("../assets/spaceship_sprite.png");
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();
        starfield.update(dt);
        spaceship.update(dt);

        window.clear(sf::Color::Black);
        starfield.draw(window);
        spaceship.draw(window);
        window.display();
    }

    return 0;
}
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

namespace client {

    struct Star {
        float x;
        float y;
        float speed;
        float size;
    };

    class Starfield {
        public:
            Starfield(float width, float height, int count = 1000);
            ~Starfield() = default;

            void update(float dt);
            void draw(sf::RenderWindow& window);

        private:
            float width_;
            float height_;
            std::vector<Star> stars_;
    };

}
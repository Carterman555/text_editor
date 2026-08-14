#pragma once

#include <cmath>

#include "SFML/Graphics.hpp"

namespace Constants {
    inline constexpr int CHARACTER_SIZE = 24;
    inline const sf::Vector2f TEXT_SHAPE_OFFSET = sf::Vector2f(0, 4);
    inline const sf::Vector2u WINDOW_SIZE = { 800, 600 };
    inline const sf::Vector2u MIN_WINDOW_SIZE = { 200, 200 };
}
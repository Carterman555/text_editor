#pragma once

#include "SFML/Graphics.hpp"

namespace Constants {
    inline constexpr int FONT_SIZE = 24;
    inline constexpr int CHARACTER_WIDTH = 13;
    inline constexpr int TAB_WIDTH = CHARACTER_WIDTH * 4;
    inline constexpr int LINE_HEIGHT = FONT_SIZE + 4;
    inline const sf::Vector2f TEXT_SHAPE_OFFSET = sf::Vector2f(0, 4);
    inline const sf::Vector2u WINDOW_SIZE = {800, 400};
}
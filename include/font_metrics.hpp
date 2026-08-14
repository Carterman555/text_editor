#pragma once

#include <SFML/Graphics.hpp>

namespace FontMetrics {
    struct Metrics {
        float line_height = 0.f;
        float char_width = 0.f;
        float tab_width = 0.f;
    };

    const Metrics& get();

    void init(const sf::Font& font, unsigned int char_size);
}
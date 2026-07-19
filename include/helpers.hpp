#pragma once

#include <SFML/Graphics.hpp>

class Helpers {
public:
    // Get the index of the character at the given world pos
    static int pos_to_char_index(const std::string& text, sf::Vector2f world_pos);

    // 
    // 
    /**
     * @brief Get the world position of the character at the given index.
     *
     * A more performant alternative to sf::Text::findCharacterPos.
     * 
     * If `index` is out of range, the position of the end of the string is returned.
     */
    static sf::Vector2f char_index_to_pos(const std::string& text, ulong index);
};
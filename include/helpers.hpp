#pragma once

#include <optional>
#include <string>
#include <SFML/Graphics.hpp>

class Helpers {
public:
    // Get the index of the character at the given world pos
    static int pos_to_char_index(const std::string& text, sf::Vector2f world_pos);

    /**
     * @brief Get the world position of the character at the given index.
     *
     * A more performant alternative to sf::Text::findCharacterPos.
     *
     * If `index` is out of range, the position of the end of the string is returned.
     */
    static sf::Vector2f char_index_to_pos(const std::string& text, std::size_t index);

    /**
     * @brief Find the size of the text area based on `text`, character widths, and line heights
     */
    static sf::Vector2f find_text_area_size(const std::string& text);

    // Filter out the characters that the text editor allows. Return nullopt if the char is not a
    // valid typable ASCII character. Also handle return by using '\n' instead of '\r'.
    static const std::optional<char> filter_char(char c);
};
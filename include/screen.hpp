#pragma once

#include <SFML/Graphics.hpp>

#include "gapbuffer.hpp"
#include "carot.hpp"
#include "selectionbox.hpp"

class Screen {
public:
    Screen();
    ~Screen();

    /**
     * @brief Get the index in the buffer of the character given a screen position
     */
    int pos_to_char_index(sf::Vector2i screen_pos);

    void on_key_pressed(sf::Keyboard::Scancode scancode);

    void type_char(int unicode);

private:
    GapBuffer buffer;

    sf::Font font;
    sf::Text text;

    Carot carot;

    SelectionBox selection_box;

    sf::RenderWindow window;
};
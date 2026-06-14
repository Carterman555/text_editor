#pragma once

#include <SFML/Graphics.hpp>

#include "gapbuffer.hpp"
#include "carot.hpp"
#include "selectionbox.hpp"

class Screen {
public:
    Screen();
    ~Screen();

    void on_key_pressed(sf::Keyboard::Scancode scancode);
    
    void type_char(int unicode);
    
    void delete_selection();

    /**
     * @brief Get the index in the buffer of the character given a screen position
     */
    int pos_to_char_index(sf::Vector2i screen_pos);




private:
    GapBuffer buffer;

    sf::Font font;
    sf::Text text;

    Carot carot;

    SelectionBox selection_box;
    int selection_start_index;

    sf::RenderWindow window;
};
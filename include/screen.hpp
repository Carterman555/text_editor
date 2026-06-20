#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

#include "gapbuffer.hpp"
#include "carot.hpp"
#include "selectionbox.hpp"
#include "view_handler.hpp"

using namespace std;

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

    int get_num_lines() {
        string str = text.getString();
        int return_count = count(str.begin(), str.end(), '\n');
        int num_lines = return_count + 1;
        return num_lines;
    }


private:
    GapBuffer buffer;

    sf::Font font;
    sf::Text text;

    Carot carot;

    SelectionBox selection_box;
    int selection_start_index;

    sf::RenderWindow window;

    ViewHandler view_handler;
};
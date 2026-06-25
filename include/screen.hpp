#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

#include "gapbuffer.hpp"
#include "caret.hpp"
#include "selectionbox.hpp"
#include "view_handler.hpp"

using namespace std;

class Screen {
public:
    Screen();
    ~Screen();

    void handle_events();

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

    void ensure_caret_visible(int pos) {
        sf::Vector2f char_pos = text.findCharacterPos(pos);
        sf::Vector2f char_center = char_pos + (TEXT_SHAPE_OFFSET / 2.f) + sf::Vector2f(CHARACTER_WIDTH / 2.f, LINE_HEIGHT / 2.f);
        view_handler.scroll_to_show_pos(char_center, get_num_lines());
    }


private:
    GapBuffer buffer;

    sf::Font font;
    sf::Text text;

    Caret caret;

    SelectionBox selection_box;
    int selection_start_index;

    sf::RenderWindow window;

    ViewHandler view_handler;
};
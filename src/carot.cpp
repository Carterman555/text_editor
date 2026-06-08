#include <iostream>
#include "carot.hpp"

using namespace std;

Carot::Carot(const sf::Text& text, int font_size) : shape(sf::Vector2f(2, font_size)), text(text) {
    shape.setPosition(carot_offset);
}

void Carot::move(int pos) {

    this->pos = pos;

    horizontal_pos = 0;
    update_shape_pos();
}

void Carot::move_left() {

    pos--;
    if (pos < 0) {
        pos = 0;
    }

    horizontal_pos = 0;
    update_shape_pos();
}

void Carot::move_right() {
    pos++;
    int char_count = text.getString().getSize();
    if (pos > char_count) {
        pos = char_count;
    }

    horizontal_pos = 0;
    update_shape_pos();
}

void Carot::move_down() {
    string str = text.getString();

    // First get the pos_in_line, which is the horizontal position of the carot. It is the amount
    // of characters before the carot in the line.	
    int pos_in_line = 0;
    int i = pos;
    while (i > 0 && str[i - 1] != '\n') {
        pos_in_line++;
        i--;
    }

    // When moving the carot to a line with less characters than the horizontal position of the
    // carot, the carot goes to the end of that line, but saves the original horizontal position.
    // Only setting the horizontal position when pos in line is greater maintains the original 
    // horizontal position 
    if (pos_in_line > horizontal_pos) {
        horizontal_pos = pos_in_line;
    }

    // Find the start of the next line
    int j = pos;
    while (str[j] != '\n') {

        // If on the last line, move carot position to end of text
        bool end_of_text = j >= str.length();
        if (end_of_text) {
            pos = str.length();

            update_shape_pos();

            return;
        }

        j++;
    }
    int next_line_start_index = j + 1;

    // Try to move the carot to the original horizontal position. If the next line is shorter than
    // this horizontal position, move the carot to the end of the line.
    for (int k = 0; k <= horizontal_pos; k++) {
        pos = next_line_start_index + k;
        if (pos >= str.length() || str[pos] == '\n') {
            break;
        }
    }

    update_shape_pos();
}

void Carot::move_up() {

    string str = text.getString();

    // First get the pos_in_line, which is the horizontal position of the carot. It is the amount
    // of characters before the carot in the line.	
    int pos_in_line = 0;
    int i = pos;
    while (i >= 0 && str[i - 1] != '\n') {
        pos_in_line++;
        i--;

        // If on the first line, move carot position to start of text
        if (i <= 0) {
            horizontal_pos = 0;
            pos = 0;
            update_shape_pos();

            return;
        }
    }

    // When moving the carot to a line with less characters than the horizontal position of the
    // carot, the carot goes to the end of that line, but saves the original horizontal position.
    // Only setting the horizontal position when pos in line is greater maintains the original 
    // horizontal position 
    if (pos_in_line > horizontal_pos) {
        horizontal_pos = pos_in_line;
    }

    // Find the start of the previous line
    int j = pos - 1;
    bool on_cur_line = true;
    while (on_cur_line || (str[j] != '\n' && j >= 0)) {

        if (str[j] == '\n') {
            on_cur_line = false;
        }

        j--;
    }
    int previous_line_start_index = j + 1;

    // Try to move the carot to the original horizontal position. If the previous line is shorter than
    // this horizontal position, move the carot to the end of the line.
    for (int k = 0; k <= horizontal_pos; k++) {
        pos = previous_line_start_index + k;
        if (pos > str.length() || str[pos] == '\n') {
            break;
        }
    }

    update_shape_pos();
}
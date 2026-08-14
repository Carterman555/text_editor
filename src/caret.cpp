#include <iostream>
#include <algorithm>

#include "caret.hpp"

Caret::Caret(GapBuffer& buffer) : shape(sf::Vector2f(2, CHARACTER_SIZE)), buffer(buffer) {
    shape.setPosition(TEXT_SHAPE_OFFSET);
}

sf::Vector2f Caret::move(int pos) {

    horizontal_pos = 0;

    this->pos = std::clamp(pos, 0, (int)buffer.get_display_str().size());

    char_pos = update_shape_pos();

    if (on_move) on_move(char_pos);

    return char_pos;
}

sf::Vector2f Caret::move_left() {

    pos--;
    if (pos < 0) {
        pos = 0;
    }

    horizontal_pos = 0;
    char_pos = update_shape_pos();

    if (on_move) on_move(char_pos);

    return char_pos;
}

sf::Vector2f Caret::move_right() {
    pos++;
    int char_count = buffer.get_display_str().size();
    if (pos > char_count) {
        pos = char_count;
    }

    horizontal_pos = 0;
    char_pos = update_shape_pos();

    if (on_move) on_move(char_pos);

    return char_pos;
}

sf::Vector2f Caret::move_down() {
    const std::string& str = buffer.get_display_str();

    // First get the pos_in_line, which is the horizontal position of the caret. It is the amount
    // of characters before the caret in the line.	
    int pos_in_line = 0;
    int i = pos;
    while (i > 0 && str[i - 1] != '\n') {
        pos_in_line++;
        i--;
    }

    // When moving the caret to a line with less characters than the horizontal position of the
    // caret, the caret goes to the end of that line, but saves the original horizontal position.
    // Only setting the horizontal position when pos in line is greater maintains the original 
    // horizontal position 
    if (pos_in_line > horizontal_pos) {
        horizontal_pos = pos_in_line;
    }

    // Find the start of the next line
    int j = pos;
    while (j == str.size() || str[j] != '\n') {

        // If on the last line, move caret position to end of text
        bool end_of_text = j >= str.size();
        if (end_of_text) {
            pos = str.size();

            char_pos = update_shape_pos();

            if (on_move) on_move(char_pos);

            return char_pos;
        }

        j++;
    }
    int next_line_start_index = j + 1;

    // Try to move the caret to the original horizontal position. If the next line is shorter than
    // this horizontal position, move the caret to the end of the line.
    for (int k = 0; k <= horizontal_pos; k++) {
        pos = next_line_start_index + k;
        if (pos >= str.length() || str[pos] == '\n') {
            break;
        }
    }

    char_pos = update_shape_pos();

    if (on_move) on_move(char_pos);

    return char_pos;
}

sf::Vector2f Caret::move_up() {

    if (pos == 0) {
        return update_shape_pos();
    }

    const std::string& str = buffer.get_display_str();

    // First get the pos_in_line, which is the horizontal position of the caret. It is the amount
    // of characters before the caret in the line.	
    int pos_in_line = 0;
    int i = pos;

    while (i > 0 && str[i - 1] != '\n') {
        pos_in_line++;
        i--;

        // If on the first line, move caret position to start of text
        if (i <= 0) {
            horizontal_pos = 0;
            pos = 0;
            char_pos = update_shape_pos();

            if (on_move) on_move(char_pos);

            return char_pos;
        }
    }

    // When moving the caret to a line with less characters than the horizontal position of the
    // caret, the caret goes to the end of that line, but saves the original horizontal position.
    // Only setting the horizontal position when pos in line is greater maintains the original 
    // horizontal position 
    if (pos_in_line > horizontal_pos) {
        horizontal_pos = pos_in_line;
    }

    // Find the start of the previous line
    int j = pos - 1;
    bool on_cur_line = true;
    while (on_cur_line || (j >= 0 && str[j] != '\n')) {

        if (str[j] == '\n') {
            on_cur_line = false;
        }

        j--;
    }
    int previous_line_start_index = j + 1;

    // Try to move the caret to the original horizontal position. If the previous line is shorter than
    // this horizontal position, move the caret to the end of the line.
    for (int k = 0; k <= horizontal_pos; k++) {
        pos = previous_line_start_index + k;
        if (pos >= str.length() || str[pos] == '\n') {
            break;
        }
    }

    char_pos = update_shape_pos();

    if (on_move) on_move(char_pos);

    return char_pos;
}

void Caret::update() {
    sf::Time time = clock.getElapsedTime();
    if (time.asSeconds() > blink_duration) {
        set_visibility(!visible);
    }
}
#include <iostream>

#include "selectionbox.hpp"
#include "constants.hpp"

using namespace Constants;

SelectionBox::SelectionBox(const sf::Text& text) : text(text) {
}

void SelectionBox::create(int start, int end) {

    this->start = start;
    this->end = end;

    int str_len = text.getString().getSize();
    start = std::clamp(start, 0, str_len);
    end = std::clamp(end, 0, str_len);

    if (start == end) {
        clear();
        return;
    }

    setup_shapes();
}

void SelectionBox::move_end(int pos) {
    end = pos;
}

void SelectionBox::clear() {
    start = -1;
    end = -1;

    shapes.clear();
}

void SelectionBox::setup_shapes() {

    std::string str = text.getString();

    int first = std::min(start, end);
    int last = std::max(start, end);

    // the index in the str where the selection starts in the previous or current line
    int prev_start_str_index = first;

    int shape_index = 0;

    // the world length of the selection in the previous or current line
    int line_selection_length = 0;

    for (int str_index = first; str_index <= last; str_index++) {

        bool end_of_str = str_index > str.size();
        if (end_of_str) {
            break;
        }

        // when reach the end of the line create or set up a rectangle shape which covers the text
        // on the previous line
        bool end_of_line = str_index >= str.size() || str[str_index] == '\n';
        bool end_of_selection = str_index == last;
        if (end_of_line || end_of_selection) {
            // create more shapes if needed
            if (shape_index >= shapes.size()) {
                shapes.push_back(std::make_unique<sf::RectangleShape>());
            }

            shapes.at(shape_index)->setSize(sf::Vector2f(line_selection_length, FONT_SIZE));
            shapes.at(shape_index)->setPosition(text.findCharacterPos(prev_start_str_index) + TEXT_SHAPE_OFFSET);
            shapes.at(shape_index)->setFillColor(sf::Color(103, 190, 217, 100));

            shape_index++;

            prev_start_str_index = str_index + 1;
            line_selection_length = 0;
        }
        else {
            int cur_char_width = str.at(str_index) == '\t' ? TAB_WIDTH : CHARACTER_WIDTH;
            line_selection_length += cur_char_width;
        }
    }

    // delete excess shapes
    while (shapes.size() > shape_index) {
        shapes.erase(shapes.begin() + shapes.size() - 1);
    }
}

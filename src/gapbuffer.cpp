#include <iostream>
#include "gapbuffer.hpp"

GapBuffer::GapBuffer(int init_gap_size) {
    this->init_gap_size = init_gap_size;
    buffer = std::vector<char>(init_gap_size, '\0');
    gap_left = 0;
    gap_right = init_gap_size - 1;
}

void GapBuffer::insert(char c) {
    if (gap_left > gap_right) {
        buffer.insert(buffer.begin() + gap_left, init_gap_size, '\0');
        gap_right += init_gap_size;
    }

    buffer.at(gap_left) = c;
    gap_left++;
}

void GapBuffer::move_gap(int position) {

    bool move_left = position < gap_left;

    if (move_left) {
        int distance = gap_left - position;

        // move the characters before the gap to after it
        buffer.insert(buffer.begin() + gap_right + 1, buffer.begin() + position, buffer.begin() + gap_left);
        buffer.erase(buffer.begin() + position, buffer.begin() + gap_left);

        gap_left = position;
        gap_right -= distance;
    }

    bool move_right = position > gap_left;
    if (move_right) {
        int distance = position - gap_right;

        // move the character after the gap to before it
        
    }
}

std::string GapBuffer::get_str() {

    std::string str;

    for (size_t i = 0; i < buffer.size(); i++) {
        if (buffer.at(i) == '\0') {
            str.push_back('_');
        }
        else {
            str.push_back(buffer.at(i));
        }
    }

    return str;
}
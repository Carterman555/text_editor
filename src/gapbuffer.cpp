#include <iostream>
#include "gapbuffer.hpp"

GapBuffer::GapBuffer() {
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

}

void GapBuffer::print() {

    for (size_t i = 0; i < buffer.size(); i++) {
        if (buffer.at(i) == '\0') {
            std::cout << '_';
        }
        else {
            std::cout << buffer.at(i);
        }
    }

    std::cout << std::endl;
}
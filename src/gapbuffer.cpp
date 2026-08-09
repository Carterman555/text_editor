#include <iostream>
#include "gapbuffer.hpp"

#include <algorithm>
#include <cassert>

GapBuffer::GapBuffer(int init_gap_size) {
    this->init_gap_size = init_gap_size;
    buffer = std::vector<char>(init_gap_size, '\0');
    gap_left = 0;
    gap_right = init_gap_size - 1;
}

void GapBuffer::insert(char c, int position) {

    assert(position >= 0 && position <= buffer.size());

    if (gap_left > gap_right) {
        buffer.insert(buffer.begin() + gap_left, init_gap_size, '\0');
        gap_right += init_gap_size;
    }

    if (gap_left != position) {
        move_gap(position);
    }

    buffer.at(gap_left) = c;
    gap_left++;

    dirty = true;
}

void GapBuffer::remove(int position) {

    if (position <= 0) {
        return;
    }

    if (gap_left != position) {
        move_gap(position);
    }

    gap_left--;
    buffer.at(gap_left) = '\0';

    dirty = true;
}

void GapBuffer::move_gap(int position) {

    bool move_left = position < gap_left;
    if (move_left) {
        int distance = gap_left - position;

        // move the gap to start at `position`
        rotate(buffer.begin() + position, buffer.begin() + gap_left, buffer.begin() + gap_right + 1);

        gap_left = position;
        gap_right -= distance;
    }

    bool move_right = position > gap_left;
    if (move_right) {

        int gap_size = (gap_right - gap_left) + 1;

        // move the character after the gap to before it
        std::copy(buffer.begin() + gap_right + 1, buffer.begin() + position + gap_size, buffer.begin() + gap_left);

        int chars_moved = (position + gap_size) - (gap_right + 1);
        gap_left += chars_moved;
        gap_right = gap_left + gap_size - 1;

        std::fill(buffer.begin() + gap_left, buffer.begin() + gap_right + 1, '\0');
    }
}

const std::string& GapBuffer::get_display_str() {

    if (!dirty) {
        return display_str;
    }

    display_str.clear();
    for (size_t i = 0; i < buffer.size(); i++) {
        if (buffer.at(i) != '\0') {
            display_str.push_back(buffer.at(i));
        }
    }

    dirty = false;

    return display_str;
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
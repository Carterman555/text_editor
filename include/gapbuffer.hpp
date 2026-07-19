#pragma once

#include <vector>
#include <string>

/**
 * @brief A gap buffer for efficient text editing.
 *
 * Stores text in two segments separated by a gap, allowing fast insertions and deletions at the cursor.
 */
class GapBuffer {
public:
    GapBuffer(int init_gap_size = 50);

    /**
     * @brief Insert a character at the position
     * @param c The character to insert
     * @param position Index to insert character at
     */
    void insert(char32_t c, int position);

    /**
     * @brief Remove a character at the position
     * @param position Index of character to remove
     */
    void remove(int position);

    /**
     * @brief Get the string of the buffer without the gap
     */
    const std::string& get_display_str();

    // Methods for Testing

    /**
     * @brief Get the string of the buffer with the gap
     */
    std::string get_str();

private:
    std::vector<char> buffer;
    int gap_left;
    int gap_right;

    // the initial size of the vector and the size of the gap inserted each time the gap is filled
    // with characters
    int init_gap_size;

    // for memoization on get_display_str()
    std::string display_str;
    bool dirty = true;

    /**
     * @brief Move the buffer to the given index to begin inserting characters at that position
     * @param position The index to move the start of the buffer to
     */
    void move_gap(int position);
};

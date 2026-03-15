#pragma once
#include <vector>

/**
 * @brief A gap buffer for efficient text editing.
 *
 * Stores text in two segments separated by a gap, allowing fast insertions and deletions at the cursor.
 */
class GapBuffer {
public:
    GapBuffer();

    /**
     * @brief insert a character at the buffer
     * @param c the character to insert
     */
    void insert(char c);

    /**
     * @brief prints the buffer for debugging
     */
    void print();

    /**
     * @brief move the buffer to the given index to begin inserting characters at that position
     * @param position the index to move the start of the buffer to
     */
    void move_gap(int position);
private:
    std::vector<char> buffer;
    int gap_left;
    int gap_right;

    // the initial size of the vector and the size of the gap inserted each time the gap is filled
    // with characters
    int init_gap_size = 10;
};

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
    GapBuffer(int init_gap_size = 10);

    /**
     * @brief insert a character at the buffer
     * @param c the character to insert
     */
    void insert(char c);

    /**
     * @brief move the buffer to the given index to begin inserting characters at that position
     * @param position the index to move the start of the buffer to
     */
    void move_gap(int position);


    // Methods for Testing

    /**
     * @brief get the string of the buffer with the gap
     */
    std::string get_str();

private:
    std::vector<char> buffer;
    int gap_left;
    int gap_right;

    // the initial size of the vector and the size of the gap inserted each time the gap is filled
    // with characters
    int init_gap_size;
};

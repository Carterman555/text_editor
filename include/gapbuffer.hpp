#pragma once

/**
 * @brief A gap buffer for efficient text editing.
 * 
 * Stores text in two segments separated by a gap, allowing fast insertions and deletions at the cursor.
 */
class GapBuffer {
public:
    GapBuffer();

    /**
     * @brief Insert a character at the buffer.
     * @param c The character to insert.
     */
    void insert(char c);
};

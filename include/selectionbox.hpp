#pragma once

#include <SFML/Graphics.hpp>

/**
 * @brief The multicharacter selection. For example, when dragging mouse over text.
 */
class SelectionBox {
public:

    SelectionBox(const sf::Text& text);

    /**
     * @brief Creates the selection with a start and end positions
     * @param start The character index of the side of the selection without the carot
     * @param end The character index of the side of the selection with the carot
     * 
     * start can be greater than end
     */
    void create(int start, int end);

    /**
     * @brief Moves the end position of the selection (the side with the carot)
     * @param pos The new end index
     */
    void move_end(int pos);

    /**
     * @brief Removes the selection box
     */
    void clear();

    sf::RectangleShape get_shape() {
        return shape;
    }

private:
    // The character index of the side of the selection without the carot
    int start;

    // The character index of the side of the selection with the carot
    int end;

    sf::RectangleShape shape;

    const sf::Text& text;
};
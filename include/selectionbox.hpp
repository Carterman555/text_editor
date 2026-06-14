#pragma once

#include <bits/stdc++.h>
#include <vector>
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

    void draw(sf::RenderWindow& window) {
        for (int i = 0; i < shapes.size(); i++) {
            window.draw(*shapes.at(i));
        }
    }

    int get_start() {
        return start;
    }

    int get_end() {
        return end;
    }

    int get_first() {
        return std::min(start, end);
    }

    int get_last() {
        return std::max(start, end);
    }

    int is_reversed() {
        return start > end;
    }

    bool is_active() {
        return start != -1 && end != -1;
    }

private:

    // The character index of the side of the selection without the carot
    int start = -1;

    // The character index of the side of the selection with the carot
    int end = -1;

    std::vector<std::unique_ptr<sf::RectangleShape>> shapes;

    const sf::Text& text;

    /**
     * @brief Create and arrange the box selection rectangle shapes
     */
    void setup_shapes();
};





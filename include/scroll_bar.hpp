#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class ScrollView;

enum Axis {
    X,
    Y
};

class ScrollBar {
public:
    ScrollBar(sf::RenderWindow& window, Axis axis);

    /**
     * @brief Start moving the scroll bar with the mouse, maintaining the same offset it had when
     * starting the drag. If the mouse is not over the scroll bar, set the offset so it drags the
     * center of the scroll bar.
     */
    void start_dragging_scroll_bar();

    void handle_dragging(ScrollView& scroll_view);

    /**
     * @brief Stop moving the scroll bar with the mouse
     */
    void stop_dragging_scroll_bar();

    /**
     * @brief Calculate the scroll bar height and position, then draw it.
     *
     * @param size_ratio The ratio of content that is visible in the window (window size / content size). Value between 0 and 1
     * @param pos_ratio How scrolled the content is (window position / window position when fully scrolled). Value between 0 and 1
     */
    void draw(float size_ratio, float pos_ratio);

    /**
     * @brief Set the viewport size and position based on the window size
     *
     * as the screen size decreases, the width ratio of the scroll bars need to increase to maintain
     * the same absolute width and vice versa.
     */
    void update_viewport();

    /**
     * @brief Convert the given world position to a value 0 to 1 based on how far the given pos is
     * on the scroll bar area
     *
     * @param world_pos x position for horizontal bar, y position for vertical bar
     */
    float world_pos_to_ratio(int world_pos) const;

    template <typename T>
    float along(T vector) const { return axis == Axis::X ? vector.x : vector.y; } // parallel to scrollbar

    template <typename T>
    float across(T vector) const { return axis == Axis::X ? vector.y : vector.x; } // perpendicular to scrollbar

    sf::Vector2f make_vec(float along, float across) const { return axis == Axis::X ? sf::Vector2f(along, across) : sf::Vector2f(across, along); }

    // also true if mouse is in the scroll area padding
    bool mouse_in_scroll_area() const {

        if (window.getViewport(view).size.x == 0 || window.getViewport(view).size.y == 0) {
            return false;
        }

        return across(window.mapPixelToCoords(sf::Mouse::getPosition(window), view)) >= 0;
    }

    bool mouse_over_scroll_bar() const {

        if (window.getViewport(view).size.x == 0 || window.getViewport(view).size.y == 0) {
            return false;
        }

        sf::Vector2f mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);

        sf::FloatRect bounds = shape.getGlobalBounds();
        return shape.getGlobalBounds().contains(mouse_pos);
    }

    bool is_dragging() const { return dragging; }

private:

    sf::RenderWindow& window;

    Axis axis;

    sf::View view;
    sf::RectangleShape shape;

    const int width = 12;
    const int length_padding = 32;
    const int width_padding = 6;

    bool dragging = false; // is dragging the scroll bar with the mouse
    int drag_offset; // world offset the mouse is from the left/top of the scroll bar when dragging
};
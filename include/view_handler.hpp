#pragma once

#include <SFML/Graphics.hpp>

#include "constants.hpp"

class ViewHandler {
public:

    ViewHandler(sf::RenderWindow& window);

    /**
     * @brief show more and prevent stretching when resizing window
     * @param new_size the size of the window in pixels after resizing.
     */
    void handle_window_resize(sf::Vector2u new_size);

    /**
     * @brief Scroll the view vertically by a given delta.
     *
     * @param delta the mouse scroll delta, not the view scroll delta
     */
    void scroll_vertically(float delta, int num_lines);

    void draw_vertical_scroll_bar(sf::RenderWindow& window, int num_lines);

    sf::View& get_text_view() { return text_view; }
    sf::View& get_vertical_scroll_view() { return vertical_scroll_view; }
    sf::View& get_horizontal_scroll_view() { return horizontal_scroll_view; }

private:
    sf::RenderWindow& window;

    sf::View text_view;
    sf::View vertical_scroll_view;
    sf::View horizontal_scroll_view;

    const float scroll_bar_width_ratio = 0.02f;
    const float vert_scroll_bar_width = scroll_bar_width_ratio * Constants::WINDOW_SIZE.x;

    sf::FloatRect view_rect;
    float zoom = 2; // zoom is greater when more zoomed in
};
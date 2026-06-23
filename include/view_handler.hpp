#pragma once

#include <SFML/Graphics.hpp>

#include "constants.hpp"

class ViewHandler {
public:

    ViewHandler(sf::RenderWindow& window);

    void update(int num_lines);

    /**
     * @brief Scroll the view vertically by a given delta.
     *
     * @param delta the mouse scroll delta, not the view scroll delta
     */
    void scroll_vertically(float delta, int num_lines);

    /**
     * @brief Scroll the view vertically to a pos based on the pixel pos.
     *
     * @param world_target_y desired world y pos to scroll the vertical scroll bar to
     */
    void scroll_to_vertical_pos(int world_target_y, int num_lines);

    /**
     * @brief Start moving the vertical scroll bar with the mouse, maintaining the same offset
     * it had when starting the drag. If the mouse is not over the scroll bar, set the offset
     * so it drags the center of the scroll bar.
     */
    void start_dragging_vertical_bar();

    /**
     * @brief Stop moving the vertical scroll bar with the mouse
     */
    void stop_dragging_vertical_bar();

    /**
     * @brief show more and prevent stretching when resizing window
     * @param new_size the size of the window in pixels after resizing.
     */
    void handle_window_resize(sf::Vector2u new_size);

    void draw_vertical_scroll_bar(int num_lines);

    /**
     * @brief is the given position is in the vertical scroll bar area
     */
    bool in_vertical_scroll_area(sf::Vector2i pos);

    sf::View& get_text_view() { return text_view; }
    sf::View& get_vertical_scroll_view() { return vertical_scroll_view; }
    sf::View& get_horizontal_scroll_view() { return horizontal_scroll_view; }

    /**
     * @brief is the given position is in the vertical scroll bar
     */
    bool over_vertical_bar(sf::Vector2i pos);

    bool is_drag_scrolling() {
        return dragging_vertical;
    }

private:

    sf::RenderWindow& window;

    sf::View text_view;
    sf::View vertical_scroll_view;
    sf::View horizontal_scroll_view;

    const float scroll_bar_width_ratio = 0.02f;
    const float vert_scroll_bar_width = scroll_bar_width_ratio * Constants::WINDOW_SIZE.x;

    sf::RectangleShape vertical_bar_rect;

    bool dragging_vertical = false; // is dragging the vertical scroll bar with the mouse
    int drag_offset; // the world offset the mouse is from the top of the scroll bar when dragging

    sf::FloatRect text_rect;
    float zoom = 2; // zoom is greater when more zoomed in

    // can scroll down until the last line is the at the very top, but still fully visible
    int get_max_text_view_y(int num_lines) {
        return (num_lines - 1) * Constants::LINE_HEIGHT;
    }
};
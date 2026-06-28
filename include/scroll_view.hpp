#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

#include "constants.hpp"

class ScrollView {
public:

    ScrollView(sf::RenderWindow& window);

    void update();

    /**
     * @brief Scroll the view vertically and horizontal to make sure the given position is within
     * view. Scroll the minimum amount required to show the position (plus an offset).
     *
     * @param pos the world pos to make visible in the content area by scrolling
     */
    void scroll_to_show_pos(sf::Vector2f pos, sf::Vector2f padding);

    /**
     * @brief Scroll the view vertically by a given delta.
     *
     * @param delta the mouse scroll delta, not the view scroll delta
     */
    void scroll_vertically(float delta);

    /**
     * @brief Scroll the view vertically to a pos based on the pixel pos.
     *
     * @param world_target_y desired world y pos to scroll the vertical scroll bar to
     */
    void scroll_bar_to_vertical_pos(int world_target_ys);

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

    void draw_vertical_scroll_bar();

    /**
     * @brief is the given position is in the vertical scroll bar area
     */
    bool in_vertical_scroll_area(sf::Vector2i pos);

    void set_content_size(sf::Vector2i size) {
        zoomed_content_size = sf::Vector2i(size.x * zoom, size.y * zoom);
    }

    sf::View& get_content_view() { return content_view; }
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

    sf::View content_view;
    sf::View vertical_scroll_view;
    sf::View horizontal_scroll_view;

    const float scroll_bar_width_ratio = 0.02f;
    const float vert_scroll_bar_width = scroll_bar_width_ratio * Constants::WINDOW_SIZE.x;
    const float hor_scroll_bar_height = scroll_bar_width_ratio * Constants::WINDOW_SIZE.y;

    sf::RectangleShape vertical_bar_rect;

    bool dragging_vertical = false; // is dragging the vertical scroll bar with the mouse
    int drag_offset; // the world offset the mouse is from the top of the scroll bar when dragging

    sf::FloatRect content_view_rect;
    float zoom = 2; // zoom is greater when more zoomed in

    sf::Vector2i zoomed_content_size = { 0, 0 };

    sf::Vector2i max_content_view_pos() {
        sf::Vector2i max_pos = zoomed_content_size - (sf::Vector2i)Constants::WINDOW_SIZE;
        max_pos.x = std::max(0, max_pos.x) / zoom;
        max_pos.y = std::max(0, max_pos.y) / zoom;

        return max_pos;
    }
};
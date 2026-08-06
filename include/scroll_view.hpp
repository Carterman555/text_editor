#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

#include "scroll_bar.hpp"
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
     * @brief Scroll the view by a given delta.
     *
     * @param delta the mouse scroll delta, not the view scroll delta
     */
    void scroll_by_delta(Axis axis, float delta);

    /**
     * @brief Scroll the content so the vertical scroll bar is at the given position.
     *
     * @param world_target_y desired world y pos to move the top of the vertical scroll bar to
     */
    void set_scroll_bar_pos(Axis axis, int world_target_y);

    // show more and prevent stretching when resizing window
    void handle_window_resize();

    void zoom_in();
    void zoom_out();

    void draw();

    void set_content_size(sf::Vector2i size) {
        content_size = size;
        zoomed_content_size = sf::Vector2i(size.x * zoom, size.y * zoom);
    }

    const sf::View& get_content_view() const { return content_view; }

    ScrollBar& get_scroll_bar(Axis axis) { return axis == Axis::X ? horizontal_scroll_bar : vertical_scroll_bar; }

private:

    /**
     * @brief Scroll the view vertically and horizontal to make sure the given position is within
     * view. Scroll the minimum amount required to show the position (plus an offset).
     *
     * @param pos the world pos to make visible in the content area by scrolling
     */
    void scroll_to_show_pos(Axis axis, float pos, float padding);

    // if the content view is outside its bounds, move it inside the bounds
    void ensure_view_within_bounds();

    // Calulcate the scroll bar height and position, then draw it. If there is no scrolling
    // possible along the axis, don't draw.
    void draw_scroll_bar(Axis axis);

    sf::RenderWindow& window;

    sf::View content_view;

    sf::FloatRect content_view_rect;
    float zoom = 1.f; // zoom is greater when more zoomed in

    sf::Vector2i content_size = { 0, 0 };
    sf::Vector2i zoomed_content_size = { 0, 0 };

    sf::Vector2i max_content_view_pos() const {
        sf::Vector2i max_pos = zoomed_content_size - (sf::Vector2i)window.getSize();
        max_pos.x = std::max(0, max_pos.x) / zoom;
        max_pos.y = std::max(0, max_pos.y) / zoom;

        return max_pos;
    }

    ScrollBar horizontal_scroll_bar;
    ScrollBar vertical_scroll_bar;
};
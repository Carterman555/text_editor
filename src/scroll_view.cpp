#include <iostream>

#include "scroll_view.hpp"
#include "logger.hpp"
#include <format>

using Constants::WINDOW_SIZE;

ScrollView::ScrollView(sf::RenderWindow& window) : window(window),
horizontal_scroll_bar(window, Axis::X), vertical_scroll_bar(window, Axis::Y) {
    content_view_rect.position = { 0.f, 0.f };
    content_view_rect.size = sf::Vector2f(WINDOW_SIZE);
    content_view = sf::View(content_view_rect);
}

void ScrollView::update() {
    vertical_scroll_bar.handle_dragging(*this);
    horizontal_scroll_bar.handle_dragging(*this);
}

void ScrollView::scroll_to_show_pos(sf::Vector2f pos, sf::Vector2f padding) {
    scroll_to_show_pos(Axis::X, pos.x, padding.x);
    scroll_to_show_pos(Axis::Y, pos.y, padding.y);
}

void ScrollView::scroll_to_show_pos(Axis axis, float pos, float padding) {

    ScrollBar& sb = get_scroll_bar(axis);

    // content left or top side position
    int content_start = sb.along(content_view_rect.position);

    // content right or bot side position
    int content_end = sb.along(content_view_rect.position) + sb.along(content_view_rect.size);

    bool too_low = pos < content_start + padding; // pos is above or left of the content view (with padding)
    bool too_high = pos > content_end - padding; // pos is below or right of the content view (with padding)
    if (too_low || too_high) {

        // If axis=X, padded_pos is the x pos to set the left of the content view rect to
        // If axis=Y, padded_pos is the y pos to set the top of the content view rect to
        float padded_pos;

        // When the view is zoomed in enough and the window is small enough, the position is show
        // can be both too low and too high. If this is the case, just center it.
        if (too_low && too_high) {
            padded_pos = pos - (sb.along(content_view_rect.size) / 2.f);
        }
        else if (too_low) {
            padded_pos = pos - padding;
        }
        else if (too_high) {
            // The position of the content view to make the given pos appear near the right/bottom.
            // Subtract window height to move the position left/up.
            padded_pos = (pos + padding) - sb.along(content_view_rect.size);
        }

        padded_pos = std::clamp(padded_pos, 0.f, sb.along(max_content_view_pos()));
        content_view_rect.position = sb.make_vec(padded_pos, sb.across(content_view_rect.position));
        content_view = sf::View(content_view_rect);
    }
}

void ScrollView::ensure_view_within_bounds() {
    sf::Vector2i max_pos = max_content_view_pos();
    content_view_rect.position.x = std::clamp(content_view_rect.position.x, 0.f, (float)max_pos.x);
    content_view_rect.position.y = std::clamp(content_view_rect.position.y, 0.f, (float)max_pos.y);
    content_view = sf::View(content_view_rect);
}

void ScrollView::scroll_by_delta(Axis axis, float delta) {

    ScrollBar& scroll_bar = get_scroll_bar(axis);

    int scroll_sensitivity = 25;
    float view_scroll_delta = -delta * scroll_sensitivity;

    int pos = scroll_bar.along(content_view_rect.position) + view_scroll_delta;
    pos = std::clamp(pos, 0, (int)scroll_bar.along(max_content_view_pos()));

    content_view_rect.position = scroll_bar.make_vec(pos, scroll_bar.across(content_view_rect.position));
    content_view = sf::View(content_view_rect);
}

void ScrollView::set_scroll_bar_pos(Axis axis, int world_target_pos) {

    ScrollBar& scroll_bar = get_scroll_bar(axis);
    int content_area_pos = scroll_bar.world_pos_to_ratio(world_target_pos) * scroll_bar.along(max_content_view_pos());

    content_view_rect.position = scroll_bar.make_vec(content_area_pos, scroll_bar.across(content_view_rect.position));
    content_view = sf::View(content_view_rect);
}

void ScrollView::handle_window_resize() {
    content_view_rect.size = sf::Vector2f(window.getSize().x / zoom, window.getSize().y / zoom);
    
    ensure_view_within_bounds();

    // `ensure_view_within_bounds` runs `content_view = sf::View(content_view_rect)` so not needed
    // here, but I'm keeping it in case `ensure_view_within_bounds` implementation changes
    content_view = sf::View(content_view_rect);

    horizontal_scroll_bar.update_viewport();
    vertical_scroll_bar.update_viewport();
}

void ScrollView::zoom_in() {
    zoom *= 1.15f;
    zoom = std::min(zoom, 2.5f);

    zoomed_content_size = sf::Vector2i(content_size.x * zoom, content_size.y * zoom);
    handle_window_resize();
    ensure_view_within_bounds();
}

void ScrollView::zoom_out() {
    zoom /= 1.15f;
    zoom = std::max(zoom, 0.1f);

    zoomed_content_size = sf::Vector2i(content_size.x * zoom, content_size.y * zoom);
    handle_window_resize();
    ensure_view_within_bounds();
}

void ScrollView::draw() {
    draw_scroll_bar(Axis::X);
    draw_scroll_bar(Axis::Y);
}

void ScrollView::draw_scroll_bar(Axis axis) {

    ScrollBar& scroll_bar = get_scroll_bar(axis);
    float size_ratio = scroll_bar.along(window.getSize()) / scroll_bar.along(zoomed_content_size);

    if (scroll_bar.along(max_content_view_pos()) == 0) {
        return;
    }
    float pos_ratio = scroll_bar.along(content_view_rect.position) / scroll_bar.along(max_content_view_pos());

    scroll_bar.draw(size_ratio, pos_ratio);
}

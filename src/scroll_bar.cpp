#include <bits/stdc++.h>

#include "scroll_bar.hpp"
#include "scroll_view.hpp"
#include "constants.hpp"

using Constants::WINDOW_SIZE;

ScrollBar::ScrollBar(sf::RenderWindow& window, Axis axis) : window(window) {
    this->axis = axis;

    shape = sf::RectangleShape();

    float window_along = along(WINDOW_SIZE);
    view = sf::View(sf::FloatRect({ 0.f, 0.f }, make_vec(window_along, width)));

    float ratio = (float)width / across(WINDOW_SIZE);
    view.setViewport(sf::FloatRect(make_vec(0.f, 1.f - ratio), make_vec(1.f, ratio)));
}

void ScrollBar::start_dragging_scroll_bar() {
    dragging = true;
    if (mouse_over_scroll_bar()) {
        int mouse = along(window.mapPixelToCoords(sf::Mouse::getPosition(window), view));
        drag_offset = along(shape.getPosition()) - mouse;
    }
    else {
        // set the offset so that the center of the scroll bar goes to the mouse position
        drag_offset = -(along(shape.getSize()) / 2);
    }
}

void ScrollBar::handle_dragging(ScrollView& scroll_view) {
    if (dragging) {
        int mouse_pos = along(window.mapPixelToCoords(sf::Mouse::getPosition(window), view));
        scroll_view.set_scroll_bar_pos(axis, mouse_pos + drag_offset);
    }
}

void ScrollBar::stop_dragging_scroll_bar() {
    dragging = false;
    drag_offset = 0;
}

float ScrollBar::world_pos_to_ratio(int world_pos) const {
    int max_scroll_bar_pos = along(Constants::WINDOW_SIZE) - along(shape.getSize());

    if (max_scroll_bar_pos <= 0.f) {
        return 0.f;
    }

    float ratio = (float)world_pos / max_scroll_bar_pos;
    return std::clamp(ratio, 0.f, 1.f);
}

void ScrollBar::draw(float size_ratio, float pos_ratio) {

    float scroll_bar_length = along(WINDOW_SIZE) * size_ratio;
    shape.setSize(make_vec(scroll_bar_length, width));

    float pos = along(WINDOW_SIZE) * (1 - size_ratio) * pos_ratio;

    shape.setPosition(make_vec(pos, 0));

    shape.setFillColor(sf::Color(100, 100, 100));
    if (mouse_over_scroll_bar() || dragging) {
        shape.setFillColor(sf::Color(150, 150, 150));
    }

    sf::View old_view = window.getView();
    window.setView(view);
    window.draw(shape);
    window.setView(old_view);
}

void ScrollBar::handle_window_resize(sf::Vector2u new_size) {
    // as the screen size decreases, the width ratio of the scroll bars need to increase to maintain
    // the same absolute width and vice versa
    float new_ratio = (float)width / across(new_size);
    view.setViewport(sf::FloatRect(make_vec(0.f, 1.f - new_ratio), make_vec(1.f, new_ratio)));
}

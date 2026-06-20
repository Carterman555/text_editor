#include <iostream>
#include <bits/stdc++.h>

#include "view_handler.hpp"

using Constants::WINDOW_SIZE;

// this might not require window parameter
ViewHandler::ViewHandler(sf::RenderWindow& window) : window(window) {
    view_rect.position = { 0.f, 0.f };
    view_rect.size = sf::Vector2f(WINDOW_SIZE);
    text_view = sf::View(view_rect);

    vertical_scroll_view = sf::View(sf::FloatRect({ 0.f, 0.f }, sf::Vector2f(vert_scroll_bar_width, WINDOW_SIZE.y)));
    horizontal_scroll_view = sf::View(sf::FloatRect({ 0.f, 0.f }, { 0.f, 0.f }));

    vertical_scroll_view.setViewport(sf::FloatRect({ 1.f - scroll_bar_width_ratio, 0.f }, { scroll_bar_width_ratio, 1.f }));
    horizontal_scroll_view.setViewport(sf::FloatRect({ 0.f, 1.f - scroll_bar_width_ratio }, { 1.f, scroll_bar_width_ratio }));
}

void ViewHandler::handle_window_resize(sf::Vector2u new_size) {
    view_rect.size = sf::Vector2f(new_size.x / zoom, new_size.y / zoom);
    text_view = sf::View(view_rect);

    // as the screen size decreases, the width ratio of the scroll bars need to increase to maintain
    // the same absolute width
    float new_width_ratio = (WINDOW_SIZE.x / new_size.x) * scroll_bar_width_ratio;
    vertical_scroll_view.setViewport(sf::FloatRect({ 1.0f - new_width_ratio, 0.f }, { new_width_ratio, 1.f }));
}

void ViewHandler::scroll_vertically(float delta, int num_lines) {
    int scroll_sensitivity = 25;
    float view_scroll_delta = -delta * scroll_sensitivity;

    // can scroll down until the last line is the at the very top, but still fully visible
    int max_y = (num_lines - 1) * Constants::LINE_HEIGHT;

    int y = view_rect.position.y + view_scroll_delta;
    y = std::clamp(y, 0, max_y);

    view_rect.position = sf::Vector2f(view_rect.position.x, y);

    text_view = sf::View(view_rect);
}

void ViewHandler::draw_vertical_scroll_bar(sf::RenderWindow& window, int num_lines) {

    int max_y = (num_lines - 1) * Constants::LINE_HEIGHT;

    float height_ratio = (float)WINDOW_SIZE.y / ((max_y * zoom) + WINDOW_SIZE.y);
    sf::RectangleShape rect({ vert_scroll_bar_width, WINDOW_SIZE.y * height_ratio });

    float position_ratio = view_rect.position.y / max_y;
    float y_pos = WINDOW_SIZE.y * (1 - height_ratio) * position_ratio;

    rect.setPosition({ 0, y_pos });
    window.draw(rect);
}

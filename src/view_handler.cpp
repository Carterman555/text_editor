#include <iostream>
#include <bits/stdc++.h>

#include "view_handler.hpp"

using Constants::WINDOW_SIZE;

// this might not require window parameter
ViewHandler::ViewHandler(sf::RenderWindow& window) : window(window) {
    text_rect.position = { 0.f, 0.f };
    text_rect.size = sf::Vector2f(WINDOW_SIZE);
    text_view = sf::View(text_rect);

    vertical_scroll_view = sf::View(sf::FloatRect({ 0.f, 0.f }, sf::Vector2f(vert_scroll_bar_width, WINDOW_SIZE.y)));
    horizontal_scroll_view = sf::View(sf::FloatRect({ 0.f, 0.f }, { 0.f, 0.f }));

    vertical_scroll_view.setViewport(sf::FloatRect({ 1.f - scroll_bar_width_ratio, 0.f }, { scroll_bar_width_ratio, 1.f }));
    horizontal_scroll_view.setViewport(sf::FloatRect({ 0.f, 1.f - scroll_bar_width_ratio }, { 1.f, scroll_bar_width_ratio }));
}

void ViewHandler::update(int num_lines) {
    if (dragging_vertical) {
        int world_mouse_y = window.mapPixelToCoords(sf::Mouse::getPosition(window), vertical_scroll_view).y;
        scroll_bar_to_vertical_pos(world_mouse_y + drag_offset, num_lines);
    }
}

void ViewHandler::scroll_to_show_pos(sf::Vector2f pos, int num_lines) {

    // scroll vertically to pos
    int text_area_top = text_rect.position.y;
    int text_area_bot = text_rect.position.y + text_rect.size.y;

    int y_padding = Constants::LINE_HEIGHT * 2.5;

    if (pos.y - y_padding < text_area_top) {
        float y = pos.y - y_padding;
        y = std::clamp(y, 0.f, (float)get_max_text_view_y(num_lines));
        text_rect.position = sf::Vector2f(text_rect.position.x, y);
    }
    else if (pos.y + y_padding > text_area_bot) {
        // The y position of the text view to make the given y pos appear near the bottom. Subtract
        // window height to move the position up.
        float y = ((pos.y + y_padding) - (float)Constants::WINDOW_SIZE.y / zoom);
        y = std::clamp(y, 0.f, (float)get_max_text_view_y(num_lines));
        text_rect.position = sf::Vector2f(text_rect.position.x, y);
    }

    // TODO - scroll horizontally to pos


    text_view = sf::View(text_rect);
}

void ViewHandler::scroll_vertically(float delta, int num_lines) {
    int scroll_sensitivity = 25;
    float view_scroll_delta = -delta * scroll_sensitivity;

    int y = text_rect.position.y + view_scroll_delta;
    y = std::clamp(y, 0, get_max_text_view_y(num_lines));

    text_rect.position = sf::Vector2f(text_rect.position.x, y);
    text_view = sf::View(text_rect);
}

void ViewHandler::scroll_bar_to_vertical_pos(int world_target_y, int num_lines) {

    int max_scroll_bar_y = Constants::WINDOW_SIZE.y - vertical_bar_rect.getSize().y;

    int scroll_bar_y = std::clamp(world_target_y, 0, max_scroll_bar_y);

    // value between 0 and 1, which is how far to scroll to. 0 = top, 1 = bottom
    float position_ratio = (float)scroll_bar_y / max_scroll_bar_y;

    int text_area_y = position_ratio * get_max_text_view_y(num_lines);

    text_rect.position = sf::Vector2f(text_rect.position.x, text_area_y);
    text_view = sf::View(text_rect);
}

void ViewHandler::start_dragging_vertical_bar() {
    dragging_vertical = true;
    if (over_vertical_bar(sf::Mouse::getPosition(window))) {
        int mouse_y = window.mapPixelToCoords(sf::Mouse::getPosition(window), vertical_scroll_view).y;
        drag_offset = vertical_bar_rect.getPosition().y - mouse_y;
    }
    else {
        // set the offset so that the center of the scroll bar goes to the mouse position
        drag_offset = -(vertical_bar_rect.getSize().y / 2);
    }
}

void ViewHandler::stop_dragging_vertical_bar() {
    dragging_vertical = false;
    drag_offset = 0;
}

void ViewHandler::handle_window_resize(sf::Vector2u new_size) {
    text_rect.size = sf::Vector2f(new_size.x / zoom, new_size.y / zoom);
    text_view = sf::View(text_rect);

    // as the screen size decreases, the width ratio of the scroll bars need to increase to maintain
    // the same absolute width
    float new_width_ratio = ((float)WINDOW_SIZE.x / new_size.x) * scroll_bar_width_ratio;
    vertical_scroll_view.setViewport(sf::FloatRect({ 1.0f - new_width_ratio, 0.f }, { new_width_ratio, 1.f }));
}

void ViewHandler::draw_vertical_scroll_bar(int num_lines) {

    float height_ratio = (float)WINDOW_SIZE.y / ((get_max_text_view_y(num_lines) * zoom) + WINDOW_SIZE.y);
    float vert_scroll_bar_height = WINDOW_SIZE.y * height_ratio;
    vertical_bar_rect = sf::RectangleShape({ vert_scroll_bar_width, vert_scroll_bar_height });

    float position_ratio = text_rect.position.y / get_max_text_view_y(num_lines);
    float y_pos = WINDOW_SIZE.y * (1 - height_ratio) * position_ratio;

    vertical_bar_rect.setPosition({ 0, y_pos });

    vertical_bar_rect.setFillColor(sf::Color(100, 100, 100));
    if (over_vertical_bar(sf::Mouse::getPosition(window)) || dragging_vertical) {
        vertical_bar_rect.setFillColor(sf::Color(150, 150, 150));
    }

    window.draw(vertical_bar_rect);
}

bool ViewHandler::in_vertical_scroll_area(sf::Vector2i screen_pos) {
    bool in_scroll_area = window.mapPixelToCoords(screen_pos, vertical_scroll_view).x >= 0;
    return in_scroll_area;
}

bool ViewHandler::over_vertical_bar(sf::Vector2i screen_pos) {
    if (!in_vertical_scroll_area(screen_pos)) {
        return false;
    }

    sf::Vector2f world_pos = window.mapPixelToCoords(screen_pos, vertical_scroll_view);

    int bar_top = vertical_bar_rect.getPosition().y;
    int bar_bot = vertical_bar_rect.getPosition().y + vertical_bar_rect.getSize().y;

    return world_pos.y > bar_top && world_pos.y < bar_bot;
}

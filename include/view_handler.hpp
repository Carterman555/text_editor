#pragma once

#include <SFML/Graphics.hpp>

#include "constants.hpp"

class ViewHandler {
public:

    ViewHandler(sf::RenderWindow& window) : window(window) {
        view_rect.position = { 0, 0 };
        view_rect.size = sf::Vector2f(Constants::WINDOW_SIZE);
        sf::View view(view_rect);
        window.setView(view);
    }

    /**
     * @brief show more and prevent stretching when resizing window
     */
    void handle_window_resize(sf::Vector2u new_size) {
        view_rect.size = sf::Vector2f(new_size.x / zoom, new_size.y / zoom);
        window.setView(sf::View(view_rect));
    }

    /**
     * @brief Scroll the view vertically by a given delta.
     *
     * @param delta the mouse scroll delta, not the view scroll delta
     */
    void scroll_vertically(float delta);

private:
    sf::RenderWindow& window;

    sf::FloatRect view_rect;
    float zoom = 1; // zoom is greater when more zoomed in
};
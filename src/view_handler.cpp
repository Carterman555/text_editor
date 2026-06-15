#include <iostream>

#include "view_handler.hpp"

void ViewHandler::scroll_vertically(float delta) {
    int scroll_sensitivity = 25;
    float view_scroll_delta = -delta * scroll_sensitivity;
    view_rect.position = sf::Vector2f(view_rect.position.x, view_rect.position.y + view_scroll_delta);

    sf::View view(view_rect);
    window.setView(view);
}
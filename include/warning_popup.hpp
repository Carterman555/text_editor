#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class WarningPopup {
public:
    WarningPopup();

    void show(const std::string& message);

    bool is_open() { return window.isOpen(); }

    // draw the popup contents and display the window
    void display();

    void set_on_close(std::function<void()> callback) {
        on_close = callback;
    }

private:
    // a rounded rectangle button that fades and lifts on hover, and sinks when pressed
    struct Button {
        sf::FloatRect bounds;
        std::string label;
        bool filled = false; // filled buttons use the accent color, ghost buttons only an outline

        // animation state, all eased towards their targets every frame
        float hover = 0.f;
        float press = 0.f;
        bool hovered = false;
        bool held = false;
    };

    void layout();
    void update_buttons(float dt);
    void draw_button(const Button& button);

    // builds a rounded rectangle as a convex shape, since sfml has no built in one
    static sf::ConvexShape rounded_rect(sf::Vector2f pos, sf::Vector2f size, float radius);

    sf::RenderWindow window;
    sf::Font font;
    sf::Text text{ font };
    sf::Text title{ font };

    Button close_button;
    Button ok_button;

    sf::Clock frame_clock;

    std::function<void()> on_close;
};
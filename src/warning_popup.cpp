#include <cmath>
#include <string>
#include <iostream>

#include "warning_popup.hpp"

namespace {
    constexpr unsigned WINDOW_WIDTH = 500;
    constexpr unsigned WINDOW_HEIGHT = 230;

    constexpr float PADDING = 26.f;
    constexpr float BUTTON_WIDTH = 120.f;
    constexpr float BUTTON_HEIGHT = 34.f;
    constexpr float BUTTON_GAP = 12.f;
    constexpr float BUTTON_RADIUS = 6.f;

    const sf::Color BACKGROUND(28, 28, 30);
    const sf::Color ACCENT(226, 170, 74);
    const sf::Color TITLE_COLOR(240, 240, 242);
    const sf::Color MESSAGE_COLOR(168, 168, 174);
    const sf::Color GHOST_COLOR(120, 120, 126);

    // how quickly the hover and press animations catch up to their targets
    constexpr float HOVER_SPEED = 14.f;
    constexpr float PRESS_SPEED = 22.f;

    // move value towards target at a rate independent of the frame length
    float ease(float value, float target, float speed, float dt) {
        return value + (target - value) * (1.f - std::exp(-speed * dt));
    }

    sf::Color mix(sf::Color a, sf::Color b, float t) {
        auto channel = [t](std::uint8_t x, std::uint8_t y) {
            return static_cast<std::uint8_t>(x + (y - x) * t);
            };
        return sf::Color(channel(a.r, b.r), channel(a.g, b.g), channel(a.b, b.b), channel(a.a, b.a));
    }
}

WarningPopup::WarningPopup() {
    text.setCharacterSize(13);
    text.setFillColor(MESSAGE_COLOR);
    text.setLineSpacing(1.4f);

    title.setCharacterSize(19);
    title.setFillColor(TITLE_COLOR);
    title.setString("Warning");

    close_button.label = "Close Editor";
    ok_button.label = "OK";
    ok_button.filled = true;

    layout();
}

void WarningPopup::layout() {
    title.setPosition({ PADDING, PADDING - 4.f });
    text.setPosition({ PADDING, PADDING + 34.f });

    const float buttons_y = WINDOW_HEIGHT - PADDING - BUTTON_HEIGHT;
    const float ok_x = WINDOW_WIDTH - PADDING - BUTTON_WIDTH;

    ok_button.bounds = { { ok_x, buttons_y }, { BUTTON_WIDTH, BUTTON_HEIGHT } };
    close_button.bounds = { { ok_x - BUTTON_GAP - BUTTON_WIDTH, buttons_y }, { BUTTON_WIDTH, BUTTON_HEIGHT } };
}

void WarningPopup::show(const std::string& message) {
    window.create(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Warning",
        sf::Style::Titlebar | sf::Style::Close);
    // the titlebar style already omits Resize, but some window managers ignore that,
    // so pin the size range too
    window.setFramerateLimit(60);
    window.setMinimumSize(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT));
    window.setMaximumSize(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT));

    text.setString(message);
    window.requestFocus();
    frame_clock.restart();
}

void WarningPopup::update_buttons(float dt) {
    const sf::Vector2f mouse(sf::Mouse::getPosition(window));

    for (Button* button : { &close_button, &ok_button }) {
        button->hovered = button->bounds.contains(mouse);
        button->hover = ease(button->hover, button->hovered ? 1.f : 0.f, HOVER_SPEED, dt);
        button->press = ease(button->press, (button->held && button->hovered) ? 1.f : 0.f, PRESS_SPEED, dt);
    }
}

sf::ConvexShape WarningPopup::rounded_rect(sf::Vector2f pos, sf::Vector2f size, float radius) {
    constexpr int CORNER_POINTS = 6;

    radius = std::min(radius, std::min(size.x, size.y) / 2.f);

    sf::ConvexShape shape;
    shape.setPointCount(CORNER_POINTS * 4);

    // the center each corner arc curves around, starting at the top right and going clockwise
    const sf::Vector2f centers[4] = {
        { pos.x + size.x - radius, pos.y + radius },
        { pos.x + size.x - radius, pos.y + size.y - radius },
        { pos.x + radius, pos.y + size.y - radius },
        { pos.x + radius, pos.y + radius },
    };

    std::size_t index = 0;
    for (int corner = 0; corner < 4; corner++) {
        for (int point = 0; point < CORNER_POINTS; point++) {
            const float angle = (static_cast<float>(corner) + static_cast<float>(point) / (CORNER_POINTS - 1))
                * (3.14159265f / 2.f) - (3.14159265f / 2.f);
            shape.setPoint(index++, centers[corner] + sf::Vector2f(std::cos(angle), std::sin(angle)) * radius);
        }
    }

    return shape;
}

void WarningPopup::draw_button(const Button& button) {
    // the button lifts slightly when hovered and sinks back down when pressed
    const float lift = button.hover * 2.f - button.press * 3.f;
    const sf::Vector2f pos = button.bounds.position + sf::Vector2f(0.f, -lift);

    sf::ConvexShape shape = rounded_rect(pos, button.bounds.size, BUTTON_RADIUS);

    sf::Color label_color;
    if (button.filled) {
        const sf::Color base = mix(ACCENT, sf::Color(244, 198, 118), button.hover);
        shape.setFillColor(mix(base, sf::Color(190, 138, 52), button.press));
        label_color = sf::Color(30, 24, 12);
    }
    else {
        shape.setFillColor(mix(sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 26), button.hover));
        shape.setOutlineThickness(1.f);
        shape.setOutlineColor(mix(sf::Color(70, 70, 76), GHOST_COLOR, button.hover));
        label_color = mix(GHOST_COLOR, TITLE_COLOR, button.hover);
    }
    window.draw(shape);

    sf::Text label(font, button.label, 14);
    label.setFillColor(label_color);

    const sf::FloatRect text_bounds = label.getLocalBounds();
    label.setOrigin(text_bounds.position + text_bounds.size / 2.f);
    label.setPosition(pos + button.bounds.size / 2.f);
    window.draw(label);
}

void WarningPopup::display() {
    // the popup owns its own window, so it has to drain its own events to stay responsive
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            return;
        }
        else if (event->is<sf::Event::Resized>()) {
            // some window managers let the window be resized despite the style, so force it back
            window.setSize({ WINDOW_WIDTH, WINDOW_HEIGHT });
        }
        else if (const auto* pressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (pressed->button == sf::Mouse::Button::Left) {
                close_button.held = close_button.bounds.contains(sf::Vector2f(pressed->position));
                ok_button.held = ok_button.bounds.contains(sf::Vector2f(pressed->position));
            }
        }
        else if (const auto* released = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (released->button == sf::Mouse::Button::Left) {
                if (close_button.held) {
                    close_button.held = false;
                    if (on_close) on_close();
                    return;
                }

                if (ok_button.held) {
                    ok_button.held = false;
                    window.close();
                    return;
                }
            }
        }
    }

    update_buttons(frame_clock.restart().asSeconds());

    window.clear(BACKGROUND);

    // accent stripe down the left edge to tie the popup to its warning color
    sf::RectangleShape stripe({ 4.f, static_cast<float>(WINDOW_HEIGHT) });
    stripe.setFillColor(ACCENT);
    window.draw(stripe);

    window.draw(title);
    window.draw(text);

    // hairline separating the message from the buttons
    sf::RectangleShape divider({ WINDOW_WIDTH - PADDING * 2.f, 1.f });
    divider.setPosition({ PADDING, WINDOW_HEIGHT - PADDING * 2.f - BUTTON_HEIGHT });
    divider.setFillColor(sf::Color(56, 56, 60));
    window.draw(divider);

    draw_button(close_button);
    draw_button(ok_button);

    window.display();
}
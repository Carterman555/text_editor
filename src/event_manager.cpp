#include <iostream>
#include <optional>

#include "helpers.hpp"
#include "event_manager.hpp"

std::vector<Event> EventManager::poll_events() {
    std::vector<Event> events;

    bool ctrl_down = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl);

    while (const std::optional event = window.pollEvent()) {

        if (event->is<sf::Event::Closed>()) {
            events.emplace_back(WindowClosed());
        }
        else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            events.emplace_back(WindowResized());
        }
        else if (const auto* key_pressed = event->getIf<sf::Event::KeyPressed>()) {

            if (key_pressed->scancode == sf::Keyboard::Scancode::Escape) {
                events.emplace_back(Escape());
            }
            else if (key_pressed->scancode == sf::Keyboard::Scancode::Backspace) {
                events.emplace_back(Backspace());
            }
            else if (key_pressed->scancode == sf::Keyboard::Scancode::Delete) {
                events.emplace_back(Delete());
            }

            add_command_events(key_pressed, events);

            if (key_pressed->scancode == sf::Keyboard::Scancode::Left ||
                key_pressed->scancode == sf::Keyboard::Scancode::Right ||
                key_pressed->scancode == sf::Keyboard::Scancode::Up ||
                key_pressed->scancode == sf::Keyboard::Scancode::Down
                ) {
                events.emplace_back(ArrowKey(key_pressed->shift, key_pressed->scancode));
            }
        }
        else if (const auto* text_entered = event->getIf<sf::Event::TextEntered>()) {
            if (const auto c = Helpers::get_valid_char(text_entered->unicode)) {
                if (!ctrl_down) events.emplace_back(TypeChar(*c));
            }
        }
        else if (const auto* mouse_data = event->getIf<sf::Event::MouseMoved>()) {
            events.emplace_back(MouseMoved(mouse_data->position));
        }
        else if (const auto* mouse_data = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouse_data->button == sf::Mouse::Button::Left) {

                events.emplace_back(LeftClick(mouse_data->position));

                // handle double and triple click
                sequential_clicks++;
                sequential_click_clock.restart();

                if (sequential_clicks == 1) {
                    events.emplace_back(SingleLeftClick(mouse_data->position));
                }
                else if (sequential_clicks % 2 == 0) {
                    events.emplace_back(DoubleLeftClick(mouse_data->position));
                }
                else {
                    events.emplace_back(TripleLeftClick(mouse_data->position));
                }
            }
        }
        else if (const auto* mouse_data = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (mouse_data->button == sf::Mouse::Button::Left) {
                events.emplace_back(LeftReleased());
            }
        }
        else if (const auto* mouse_data = event->getIf<sf::Event::MouseWheelScrolled>()) {
            events.emplace_back(MouseWheelScrolled(mouse_data->delta, ctrl_down));
        }
    }

    // handle double and triple click
    if (sequential_clicks > 0 && sequential_click_clock.getElapsedTime().asMilliseconds() > 400) {
        sequential_clicks = 0;
        sequential_click_clock.reset();
    }

    return events;
}

void EventManager::add_command_events(const sf::Event::KeyPressed* key_pressed, std::vector<Event>& events) {
    if (key_pressed->control) {
        switch (key_pressed->scancode) {
            case sf::Keyboard::Scancode::S:
                events.emplace_back(CtrlS()); break;
            case sf::Keyboard::Scancode::C:
                events.emplace_back(CtrlC()); break;
            case sf::Keyboard::Scancode::V:
                events.emplace_back(CtrlV()); break;
            case sf::Keyboard::Scancode::A:
                events.emplace_back(CtrlA()); break;
            case sf::Keyboard::Scancode::Hyphen:
                events.emplace_back(CtrlHyphen()); break;
            case sf::Keyboard::Scancode::Equal:
                events.emplace_back(CtrlEqual()); break;
            default:
                break;
        }
    }
}
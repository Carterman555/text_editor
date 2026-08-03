#pragma once

#include <variant>
#include <vector>
#include <SFML/Graphics.hpp>

struct WindowClosed {};
struct WindowResized {};
struct TypeChar { char32_t c; };
struct MouseMoved { sf::Vector2i position; };
struct LeftClick { sf::Vector2i position; };
struct SingleLeftClick { sf::Vector2i position; };
struct DoubleLeftClick { sf::Vector2i position; };
struct TripleLeftClick { sf::Vector2i position; };
struct LeftReleased {};
struct MouseWheelScrolled { float delta; bool ctrl; };

struct Escape {};
struct Backspace {};
struct Delete {};

struct CtrlS {};
struct CtrlC {};
struct CtrlV {};
struct CtrlA {};
struct CtrlHyphen {};
struct CtrlEqual {};

struct ArrowKey { bool shift; sf::Keyboard::Scancode scancode; };

using Event = std::variant<
    WindowClosed, WindowResized, TypeChar,
    MouseMoved, LeftClick, SingleLeftClick, DoubleLeftClick,
    TripleLeftClick, LeftReleased, MouseWheelScrolled,
    Escape, Backspace, Delete, CtrlS, CtrlC, CtrlV, CtrlA,
    CtrlHyphen, CtrlEqual, ArrowKey
>;

class EventManager {
public:

    EventManager(sf::RenderWindow& window) : window(window) {}

    std::vector<Event> poll_events();


private:
    void add_command_events(const sf::Event::KeyPressed* key_pressed, std::vector<Event>& events);

    sf::RenderWindow& window;

    int sequential_clicks = 0;
    sf::Clock sequential_click_clock;
};
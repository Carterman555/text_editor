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

// When the mouse is left clicked twice within a short period of time with the cursor remaining in
// the same location, a `DoubleLeftClick` event is returned. Likewise with `TripleLeftClick`.
// If there are 4 clicks in a row, it returns another `DoubleLeftClick` event. 5 in a row returns
// `TripleLeftClick`, and so on.
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

    // Interpret and return events from sf `window.pollEvent()`
    std::vector<Event> poll_events();


private:
    // Interpret all command events and add them to `events`
    void add_command_events(const sf::Event::KeyPressed* key_pressed, std::vector<Event>& events);

    sf::RenderWindow& window;

    int sequential_clicks = 0;
    sf::Clock sequential_click_clock;
};
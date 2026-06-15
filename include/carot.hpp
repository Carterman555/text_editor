#pragma once

#include <SFML/Graphics.hpp>
#include "constants.hpp"

using namespace Constants;

/**
 * @brief Handles the movement and display of the carot
 *
 * The carot is the small vertical line where text is written.
 */
class Carot {

public:

    Carot(const sf::Text& text);

    /**
     * @brief Move the carot to the given position
     * @param pos desired index of text to move the carot to
     */
    void move(int pos);

    /**
     * @brief Move the carot to the left by one
     *
     * Does nothing if carot is in the left most position
     */
    void move_left();

    /**
     * @brief Move the carot to the right by one
     *
     * Has no effect if carot is in the right most position
     */
    void move_right();

    /**
     * @brief Move the carot down to the next line, maintaining the horizontal position
     *
     * If in the last line, the carot will move to the end of the line. If at the end of the last
     * line, this has no effect.
     */
    void move_down();

    /**
     * @brief Move the carot up to the previous line, maintaining the horizontal position
     *
     * If in the first line, the carot will move to the start of the line. If at the start of the
     * first line, this has no effect.
     */
    void move_up();


    /**
     * @brief Update the position of the carot shape on the screen and reset the blink.
     *
     * Invoke when the carot position changes.
     */
    void update_shape_pos() {
        shape.setPosition(text.findCharacterPos(pos) + TEXT_SHAPE_OFFSET);
        set_visibility(true);
    }

    int get_pos() {
        return pos;
    }

    void update();

    void draw(sf::RenderWindow& window) {
        if (visible) {
            window.draw(shape);
        }
    }

    // Set visibility for the blink, and restarts the clock responsible for controlling blink
    void set_visibility(bool visible) {
        this->visible = visible;
        clock.restart();
    }

private:
    sf::RectangleShape shape;
    int pos = 0;

    // When you press the up or down arrow, the carot moves up or down. This variable saves
    // the original horizontal position.
    int horizontal_pos = 0;

    const sf::Text& text;

    // Indicates when the cursor is in the visible duration of the blinking. This varible
    // only has to do with blinking.
    bool visible = true;
    sf::Clock clock;

    // Duration of blink in seconds. Not the full duration of visible and not visible. Just the
    // duration of one state.
    const float blink_duration = 0.5;
};
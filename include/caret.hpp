#pragma once

#include <functional>
#include <SFML/Graphics.hpp>

#include "gapbuffer.hpp"
#include "constants.hpp"
#include "helpers.hpp"

using namespace Constants;

/**
 * @brief Handles the movement and display of the caret
 *
 * The caret is the small vertical line where text is written.
 */
class Caret {

public:

    Caret(GapBuffer& buffer);

    void set_on_move(std::function<void(sf::Vector2f)> callback) {
        on_move = callback;
    }

    /**
     * @brief Move the caret to the given position
     * @param pos desired index of text to move the caret to
     *
     * @return position of character
     */
    sf::Vector2f move(int pos);

    /**
     * @brief Move the caret to the left by one
     *
     * Does nothing if caret is in the left most position
     *
     * @return position of character
     */
    sf::Vector2f move_left();

    /**
     * @brief Move the caret to the right by one
     *
     * Has no effect if caret is in the right most position
     *
     * @return position of character
     */
    sf::Vector2f move_right();

    /**
     * @brief Move the caret down to the next line, maintaining the horizontal position
     *
     * If in the last line, the caret will move to the end of the line. If at the end of the last
     * line, this has no effect.
     *
     * @return position of character
     */
    sf::Vector2f move_down();

    /**
     * @brief Move the caret up to the previous line, maintaining the horizontal position
     *
     * If in the first line, the caret will move to the start of the line. If at the start of the
     * first line, this has no effect.
     *
     * @return position of character
     */
    sf::Vector2f move_up();


    /**
     * @brief Update the position of the caret shape on the screen and reset the blink.
     *
     * Invoke when the caret position changes.
     *
     * @return position of character
     */
    sf::Vector2f update_shape_pos() {
        sf::Vector2f char_pos = Helpers::char_index_to_pos(buffer.get_display_str(), pos);
        shape.setPosition(char_pos + TEXT_SHAPE_OFFSET);
        set_visibility(true);

        return char_pos;
    }

    int get_pos() const {
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
    std::function<void(sf::Vector2f)> on_move;

    sf::RectangleShape shape;
    int pos = 0;

    // When you press the up or down arrow, the caret moves up or down. This variable saves
    // the original horizontal position.
    int horizontal_pos = 0;

    sf::Vector2f char_pos;

    GapBuffer& buffer;

    // Indicates when the cursor is in the visible duration of the blinking. This varible
    // only has to do with blinking.
    bool visible = true;
    sf::Clock clock;

    // Duration of blink in seconds. Not the full duration of visible and not visible. Just the
    // duration of one state.
    const float blink_duration = 0.5;
};
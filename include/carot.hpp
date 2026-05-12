#include <SFML/Graphics.hpp>

/**
 * @brief Handles the movement and display of the carot
 *
 * The carot is the small vertical line where text is written.
 */
class Carot {
private:
    sf::RectangleShape shape;
    sf::Vector2f carot_offset = sf::Vector2f(0, 4);
    int pos = 0;

    // When you press the up or down arrow, the carot moves up or down. This variable saves
    // the original horizontal position.
    int horizontal_pos = 0;

    sf::Text text;

public:

    Carot(const sf::Text& text, int font_size);

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
     * @brief Update the position of the carot shape on the screen
     *
     * Invoke when the carot position changes
     */
    void update_shape_pos() {
        shape.setPosition(text.findCharacterPos(pos) + carot_offset);
    }

    int get_pos() {
        return pos;
    }

    sf::RectangleShape get_shape() {
        return shape;
    }
};
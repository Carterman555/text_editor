#include <SFML/Graphics.hpp>

#include "gapbuffer.hpp"

class Screen {
private:
    GapBuffer buffer;
	int carot_pos = 0;

	int font_size = 24;
	int char_width = 13;

    sf::RectangleShape carot;
public:
    Screen();
    ~Screen();

    /**
     * @brief Get the index in the buffer of the character given a screen position
     */
    int get_character_index(sf::Vector2i screen_pos);

    /**
     * @brief Update the position of the carot shape on the screen
     * 
     * Invoke when the carot position changes
     */
    void update_carot_pos(sf::Text text);
};
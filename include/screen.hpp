#include <SFML/Graphics.hpp>

#include "gapbuffer.hpp"
#include "carot.hpp"

class Screen {
private:
    GapBuffer buffer;

    int font_size = 24;
    int char_width = 13;

    Carot carot;

    sf::RenderWindow window;

    sf::Font font;
    sf::Text text;

public:
    Screen();
    ~Screen();

    /**
     * @brief Get the index in the buffer of the character given a screen position
     */
    int pos_to_char_index(sf::Vector2i screen_pos);

    void on_key_pressed(sf::Keyboard::Scancode scancode);

    void type_char(int unicode);
};
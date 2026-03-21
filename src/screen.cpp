#include <iostream>


#include "screen.hpp"

Screen::Screen() : buffer(), carot(sf::Vector2f(2, font_size)) {

	sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "My Window");
	window.setPosition({ 50, 50 });

	sf::Font font("CONSOLA.TTF");
	sf::Text text(font);

	text.setString("");
	text.setCharacterSize(font_size);
	text.setFillColor(sf::Color::White);

	carot.setPosition(sf::Vector2f(0, 4));

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape){
					window.close();
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Left) {
					carot_pos--;
					if (carot_pos < 0) {
						carot_pos = 0;
					}
					buffer.move_gap(carot_pos);

					update_carot_pos(text);
				}
			}
			else if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
				
				if (textEntered->unicode < 128) {
					char c = static_cast<char>(textEntered->unicode);
					
					const int return_unicode = 13;
					if (textEntered->unicode == return_unicode) {
						c = '\n';
					}

					// cout << c << endl;

					buffer.insert(c, carot_pos);
					carot_pos++;

					text.setString(buffer.get_display_str());
					
					update_carot_pos(text);
				}
			}
		}

		window.clear(sf::Color(25, 25, 25));

		window.draw(text);
		window.draw(carot);

		window.display();
	}
}

Screen::~Screen() {
}


int Screen::get_character_index(sf::Vector2i screen_pos) {
    /*
    Getting character from screen position.
    First, get the line number of position using the number of returns. I could also do the math using character spacing to 
    calculate the character in that line directly.
    */

	// find the line number


	// calculate the character within the line

	return 0;
}

void Screen::update_carot_pos(sf::Text text) {
	sf::Vector2f offset = sf::Vector2f(0, 4);
	carot.setPosition(text.findCharacterPos(carot_pos) + offset);
}
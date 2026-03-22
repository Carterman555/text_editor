#include <iostream>
#include <bits/stdc++.h>

#include "screen.hpp"

Screen::Screen() : buffer(), carot(sf::Vector2f(2, font_size)) {

	sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "My Window");
	window.setPosition({ 50, 50 });

	std::string projectDir = PROJECT_DIR;

	sf::Font font(projectDir + "/CONSOLA.TTF");
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
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
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
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Right) {
					carot_pos++;
					int char_count = text.getString().getSize();
					if (carot_pos > char_count) {
						carot_pos = char_count;
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

					buffer.insert(c, carot_pos);
					carot_pos++;

					text.setString(buffer.get_display_str());

					update_carot_pos(text);
				}
			}
			else if (const auto* mouseData = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (mouseData->button == sf::Mouse::Button::Left) {
					carot_pos = pos_to_char_index(mouseData->position);

					buffer.move_gap(carot_pos);
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


int Screen::pos_to_char_index(sf::Vector2i screen_pos) {

	// Step 1: get the line number, the start index of that line, and the size of the line

	int line_spacing_pixels = 4; // changes depending on the font I think
	int line_number = ceil((float)screen_pos.y / (font_size + line_spacing_pixels));

	if (line_number < 1) {
		line_number = 1;
	}

	int line_counter = 0;
	std::string text = buffer.get_display_str();

	int line_start_index = 0;
	int chars_in_line = 0;

	bool reached_line_num = false;

	std::stringstream stream(text);
	std::string line;
	while (getline(stream, line, '\n')) { // loop through each line

		chars_in_line = line.size();

		line_counter++;
		if (line_counter >= line_number) {
			reached_line_num = true;
			break;
		}

		line_start_index += chars_in_line + 1;
	}

	// If the line number was not reached this means the cursor position was below the last line.
	// When this happens, it always move the carot to after the last character
	if (!reached_line_num) {
		return text.size();
	}

	if (line_number > line_counter) {
		line_number = line_counter;
	}

	// Step 2: Use the line number, the start index of that line, and the size of the line to
	// calculate the index

	//... 0 for the first char of each line, 1 for the second, and so on
	int char_index_in_line = round(((float)screen_pos.x) / char_width);

	if (char_index_in_line > chars_in_line) {
		char_index_in_line = chars_in_line;
	}

	return line_start_index + char_index_in_line;
}

void Screen::update_carot_pos(sf::Text text) {
	sf::Vector2f offset = sf::Vector2f(0, 4);
	carot.setPosition(text.findCharacterPos(carot_pos) + offset);
}
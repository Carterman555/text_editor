#include <iostream>
#include <bits/stdc++.h>

#include "screen.hpp"

using namespace std;

Screen::Screen() : buffer(),
font((std::string)PROJECT_DIR + "/CONSOLA.TTF"),
text(font),
carot(text, font_size),
window(sf::VideoMode({ 800, 800 }), "Text Editor") {

	window.setPosition({ 50, 50 });

	text.setString("");
	text.setCharacterSize(font_size);
	text.setFillColor(sf::Color::White);

	while (window.isOpen()) {

		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				on_key_pressed(keyPressed->scancode);
			}
			else if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
				type_char(textEntered->unicode);
			}
			else if (const auto* mouseData = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (mouseData->button == sf::Mouse::Button::Left) {
					int desired_carot_pos = pos_to_char_index(mouseData->position);
					carot.move(desired_carot_pos);
					buffer.move_gap(carot.get_pos());
				}
			}
		}

		window.clear(sf::Color(25, 25, 25));

		window.draw(text);
		window.draw(carot.get_shape());

		window.display();
	}
}

Screen::~Screen() {
}

void Screen::on_key_pressed(sf::Keyboard::Scancode scancode) {
	if (scancode == sf::Keyboard::Scancode::Escape) {
		window.close();
	}
	else if (scancode == sf::Keyboard::Scancode::Left) {
		carot.move_left();
		buffer.move_gap(carot.get_pos());
	}
	else if (scancode == sf::Keyboard::Scancode::Right) {
		carot.move_right();
		buffer.move_gap(carot.get_pos());
	}
	else if (scancode == sf::Keyboard::Scancode::Down) {
		carot.move_down();
		buffer.move_gap(carot.get_pos());
	}
	else if (scancode == sf::Keyboard::Scancode::Up) {
		carot.move_up();
		buffer.move_gap(carot.get_pos());
	}
}

void Screen::type_char(int unicode) {

	const int backspace_unicode = 8;
	const int delete_unicode = 127;

	if (unicode == backspace_unicode) {

		if (carot.get_pos() <= 0) {
			return;
		}

		buffer.remove(carot.get_pos());

		text.setString(buffer.get_display_str());
		carot.move_left();
	}
	else if (unicode == delete_unicode) {
		if (carot.get_pos() >= text.getString().getSize()) {
			return;
		}

		buffer.remove(carot.get_pos() + 1);

		text.setString(buffer.get_display_str());
	}
	else if (unicode < 128) {
		char c = static_cast<char>(unicode);

		const int return_unicode = 13;
		if (unicode == return_unicode) {
			c = '\n';
		}

		buffer.insert(c, carot.get_pos());

		text.setString(buffer.get_display_str());
		carot.move_right();
	}
}

int Screen::pos_to_char_index(sf::Vector2i screen_pos) {

	// Step 1: Get the line number, the start index of that line, and the size of the line

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
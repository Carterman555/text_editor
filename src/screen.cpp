#include <iostream>

#include "screen.hpp"
#include "filehandler.hpp"
#include "constants.hpp"
#include "logger.hpp"

using namespace Constants;

Screen::Screen(string contents) : buffer(),
font((std::string)PROJECT_DIR + "/CONSOLA.TTF"),
text(font),
caret(text),
selection_box(text),
window(sf::VideoMode(WINDOW_SIZE), "Text Editor"),
scroll_view(window) {

	window.setPosition({ 50, 50 });

	text.setCharacterSize(FONT_SIZE);
	text.setFillColor(sf::Color::White);

	for (int i = 0; i < contents.size(); i++) {
		buffer.insert(contents.at(i), i);
	}
	update_text();

	caret.set_on_move([this](int pos) {ensure_caret_visible(pos);});
}

void Screen::run_window() {
	while (window.isOpen()) {
		handle_events();

		caret.update();
		scroll_view.update();

		// draw
		window.setView(scroll_view.get_content_view());

		window.clear(sf::Color(25, 25, 25));

		selection_box.draw(window);
		window.draw(text);
		caret.draw(window);

		scroll_view.draw();

		window.display();
	}
}

void Screen::handle_events() {

	while (const std::optional event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			window.close();
		}
		else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
			scroll_view.handle_window_resize();
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
			on_key_pressed(keyPressed);
		}
		else if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
			if (textEntered->unicode == 9 || (textEntered->unicode >= 32 && textEntered->unicode < 127)) {
				type_char(static_cast<char>(textEntered->unicode));
			}
			else if (textEntered->unicode == 13) {
				type_char('\n');
			}
		}
		else if (const auto* mouseData = event->getIf<sf::Event::MouseMoved>()) {
			bool drag_scrolling = scroll_view.get_scroll_bar(Axis::X).is_dragging() || scroll_view.get_scroll_bar(Axis::Y).is_dragging();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !drag_scrolling) {

				int desired_caret_pos = pos_to_char_index(mouseData->position);
				caret.move(desired_caret_pos);

				selection_box.clear();
				selection_box.create(selection_start_index, caret.get_pos());
			}
		}
		else if (const auto* mouseData = event->getIf<sf::Event::MouseButtonPressed>()) {

			if (mouseData->button == sf::Mouse::Button::Left) {
				if (scroll_view.get_scroll_bar(Axis::X).mouse_in_scroll_area()) {
					scroll_view.get_scroll_bar(Axis::X).start_dragging_scroll_bar();
				}
				else if (scroll_view.get_scroll_bar(Axis::Y).mouse_in_scroll_area()) {
					scroll_view.get_scroll_bar(Axis::Y).start_dragging_scroll_bar();
				}
				else {
					int desired_caret_pos = pos_to_char_index(mouseData->position);
					caret.move(desired_caret_pos);

					selection_start_index = caret.get_pos();
					selection_box.clear();
				}
			}
		}
		else if (const auto* mouseData = event->getIf<sf::Event::MouseButtonReleased>()) {
			if (mouseData->button == sf::Mouse::Button::Left) {
				scroll_view.get_scroll_bar(Axis::X).stop_dragging_scroll_bar();
				scroll_view.get_scroll_bar(Axis::Y).stop_dragging_scroll_bar();
			}
		}
		else if (const auto* mouseData = event->getIf<sf::Event::MouseWheelScrolled>()) {
			bool ctrl_down = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl);

			if (ctrl_down) {
				if (mouseData->delta < 0) {
					scroll_view.zoom_out();
				}
				else if (mouseData->delta > 0) {
					scroll_view.zoom_in();
				}
			}
			else {
				scroll_view.scroll_by_delta(Axis::Y, mouseData->delta);
			}
		}
	}
}

void Screen::on_key_pressed(const sf::Event::KeyPressed* keyPressed) {
	if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
		window.close();
	}
	else if (keyPressed->scancode == sf::Keyboard::Scancode::Backspace) {
		if (selection_box.is_active()) {
			delete_selection();
			return;
		}

		if (caret.get_pos() <= 0) {
			return;
		}

		buffer.remove(caret.get_pos());

		update_text();
		caret.move_left();
	}
	else if (keyPressed->scancode == sf::Keyboard::Scancode::Delete) {
		if (selection_box.is_active()) {
			delete_selection();
			return;
		}

		if (caret.get_pos() >= text.getString().getSize()) {
			return;
		}

		buffer.remove(caret.get_pos() + 1);

		update_text();
	}

	handle_commands(keyPressed);
	handle_arrow_keys(keyPressed);
}

void Screen::handle_commands(const sf::Event::KeyPressed* keyPressed) {
	if (keyPressed->control && keyPressed->scancode == sf::Keyboard::Scancode::S) {
		if (on_save) on_save(buffer.get_display_str());
	}
	else if (keyPressed->control && keyPressed->scancode == sf::Keyboard::Scancode::C) {
		if (selection_box.is_active()) {
			sf::Clipboard::setString(selection_box.get_selection());
		}
	}
	else if (keyPressed->control && keyPressed->scancode == sf::Keyboard::Scancode::V) {

		if (selection_box.is_active()) {
			delete_selection();
		}

		string clipboard = sf::Clipboard::getString();
		for (int i = 0; i < clipboard.size(); i++) {
			buffer.insert(clipboard.at(i), caret.get_pos() + i);
		}

		update_text();

		caret.move(caret.get_pos() + clipboard.size());
	}
	else if (keyPressed->control && keyPressed->scancode == sf::Keyboard::Scancode::Hyphen) {
		scroll_view.zoom_out();
	}
	else if (keyPressed->control && keyPressed->scancode == sf::Keyboard::Scancode::Equal) {
		scroll_view.zoom_in();
	}
}

void Screen::handle_arrow_keys(const sf::Event::KeyPressed* keyPressed) {
	if (keyPressed->shift && keyPressed->scancode == sf::Keyboard::Scancode::Left) {
		int selection_start = selection_box.is_active() ? selection_box.get_start() : caret.get_pos();
		caret.move_left();
		selection_box.create(selection_start, caret.get_pos());
	}
	else if (keyPressed->shift && keyPressed->scancode == sf::Keyboard::Scancode::Right) {
		int selection_start = selection_box.is_active() ? selection_box.get_start() : caret.get_pos();
		caret.move_right();
		selection_box.create(selection_start, caret.get_pos());
	}
	else if (keyPressed->shift && keyPressed->scancode == sf::Keyboard::Scancode::Up) {
		int selection_start = selection_box.is_active() ? selection_box.get_start() : caret.get_pos();
		caret.move_up();
		selection_box.create(selection_start, caret.get_pos());
	}
	else if (keyPressed->shift && keyPressed->scancode == sf::Keyboard::Scancode::Down) {
		int selection_start = selection_box.is_active() ? selection_box.get_start() : caret.get_pos();
		caret.move_down();
		selection_box.create(selection_start, caret.get_pos());
	}
	else if (keyPressed->scancode == sf::Keyboard::Scancode::Left) {
		if (!selection_box.is_active()) {
			caret.move_left();
		}
		else {
			caret.move(selection_box.get_first());
			selection_box.clear();
		}
	}
	else if (keyPressed->scancode == sf::Keyboard::Scancode::Right) {
		if (!selection_box.is_active()) {
			caret.move_right();
		}
		else {
			caret.move(selection_box.get_last());
			selection_box.clear();
		}
	}
	else if (keyPressed->scancode == sf::Keyboard::Scancode::Down) {
		if (!selection_box.is_active()) {
			caret.move_down();
		}
		else {
			caret.move(selection_box.get_last());
			caret.move_down();
			selection_box.clear();
		}
	}
	else if (keyPressed->scancode == sf::Keyboard::Scancode::Up) {
		if (!selection_box.is_active()) {
			caret.move_up();
		}
		else {
			caret.move(selection_box.get_first());
			caret.move_up();
			selection_box.clear();
		}
	}
}

void Screen::type_char(char c) {

	if (selection_box.is_active()) {
		delete_selection();
	}

	buffer.insert(c, caret.get_pos());

	update_text();
	caret.move_right();
}

void Screen::delete_selection() {

	caret.move(selection_box.get_first());

	// repeatedly remove the first character in the selection from the buffer. Since the element is
	// removed, each loop iteration removes the next character in order.
	int selection_length = selection_box.get_last() - selection_box.get_first();
	for (int i = 0; i < selection_length; i++) {
		buffer.remove(selection_box.get_first() + 1);
	}
	selection_box.clear();

	update_text();
}

int Screen::pos_to_char_index(sf::Vector2i screen_pos) {

	sf::Vector2f world_pos = window.mapPixelToCoords(screen_pos, scroll_view.get_content_view());

	// Step 1: Get the line number, the start index of that line, and the size of the line
	int line_number = ceil(world_pos.y / LINE_HEIGHT);

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
	// When this happens, it always move the caret to after the last character
	if (!reached_line_num) {
		return text.size();
	}

	if (line_number > line_counter) {
		line_number = line_counter;
	}

	// Step 2: Use the line number, the start index of that line, and the size of the line to
	// calculate the index. Go through each char in the line until it reaches the world x to
	// get the char_index_in_line.

	//... 0 for the first char of each line, 1 for the second, and so on
	int char_index_in_line;

	int current_x = 0;
	for (char_index_in_line = 0; char_index_in_line < chars_in_line; char_index_in_line++) {
		int char_index = line_start_index + char_index_in_line;

		int cur_char_width = text.at(char_index) == '\t' ? TAB_WIDTH : CHARACTER_WIDTH;
		current_x += cur_char_width;

		if (current_x > world_pos.x + (cur_char_width / 2.f)) {
			break;
		}
	}

	return line_start_index + char_index_in_line;
}
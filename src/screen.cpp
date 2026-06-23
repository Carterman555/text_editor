#include <iostream>

#include "screen.hpp"
#include "constants.hpp"

using namespace Constants;

Screen::Screen() : buffer(),
font((std::string)PROJECT_DIR + "/CONSOLA.TTF"),
text(font),
carot(text),
selection_box(text),
window(sf::VideoMode(WINDOW_SIZE), "Text Editor"),
view_handler(window) {

	window.setPosition({ 50, 50 });

	text.setString("");
	text.setCharacterSize(FONT_SIZE);
	text.setFillColor(sf::Color::White);

	while (window.isOpen()) {

		handle_events();

		carot.update();
		view_handler.update(get_num_lines());

		// draw
		window.setView(view_handler.get_text_view());

		window.clear(sf::Color(25, 25, 25));

		selection_box.draw(window);
		window.draw(text);
		carot.draw(window);

		window.setView(view_handler.get_vertical_scroll_view());
		view_handler.draw_vertical_scroll_bar(get_num_lines());

		window.setView(view_handler.get_horizontal_scroll_view());

		window.display();

		sf::Vector2i mouse_screen_pos = sf::Mouse::getPosition(window);
		sf::Vector2f mouse_world_pos = window.mapPixelToCoords(mouse_screen_pos, view_handler.get_vertical_scroll_view());
	}
}

Screen::~Screen() {
}

void Screen::handle_events() {
	while (const std::optional event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			window.close();
		}
		else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
			view_handler.handle_window_resize(resized->size);
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
			on_key_pressed(keyPressed->scancode);
		}
		else if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
			type_char(textEntered->unicode);
		}
		else if (const auto* mouseData = event->getIf<sf::Event::MouseMoved>()) {
			if (!view_handler.is_drag_scrolling()) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

					int desired_carot_pos = pos_to_char_index(mouseData->position);
					carot.move(desired_carot_pos);

					selection_box.clear();
					selection_box.create(selection_start_index, carot.get_pos());
				}
			}
		}
		else if (const auto* mouseData = event->getIf<sf::Event::MouseButtonPressed>()) {

			if (mouseData->button == sf::Mouse::Button::Left) {
				bool cursor_in_text_area = !view_handler.in_vertical_scroll_area(sf::Mouse::getPosition(window));
				if (cursor_in_text_area) {
					int desired_carot_pos = pos_to_char_index(mouseData->position);
					carot.move(desired_carot_pos);

					selection_start_index = carot.get_pos();
					selection_box.clear();
				}
				else {
					view_handler.start_dragging_vertical_bar();
				}
			}
		}
		else if (const auto* mouseData = event->getIf<sf::Event::MouseButtonReleased>()) {
			if (mouseData->button == sf::Mouse::Button::Left) {
				view_handler.stop_dragging_vertical_bar();
			}
		}
		else if (const auto* mouseData = event->getIf<sf::Event::MouseWheelScrolled>()) {
			view_handler.scroll_vertically(mouseData->delta, get_num_lines());
		}
	}
}

void Screen::on_key_pressed(sf::Keyboard::Scancode scancode) {

	bool shift = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift);

	if (scancode == sf::Keyboard::Scancode::Escape) {
		window.close();
	}
	else if (shift && scancode == sf::Keyboard::Scancode::Left) {
		int selection_start = selection_box.is_active() ? selection_box.get_start() : carot.get_pos();
		carot.move_left();
		selection_box.create(selection_start, carot.get_pos());
	}
	else if (shift && scancode == sf::Keyboard::Scancode::Right) {
		int selection_start = selection_box.is_active() ? selection_box.get_start() : carot.get_pos();
		carot.move_right();
		selection_box.create(selection_start, carot.get_pos());
	}
	else if (shift && scancode == sf::Keyboard::Scancode::Up) {
		int selection_start = selection_box.is_active() ? selection_box.get_start() : carot.get_pos();
		carot.move_up();
		selection_box.create(selection_start, carot.get_pos());
	}
	else if (shift && scancode == sf::Keyboard::Scancode::Down) {
		int selection_start = selection_box.is_active() ? selection_box.get_start() : carot.get_pos();
		carot.move_down();
		selection_box.create(selection_start, carot.get_pos());
	}
	else if (scancode == sf::Keyboard::Scancode::Left) {
		if (!selection_box.is_active()) {
			carot.move_left();
		}
		else {
			carot.move(selection_box.get_first());
			selection_box.clear();
		}
	}
	else if (scancode == sf::Keyboard::Scancode::Right) {
		if (!selection_box.is_active()) {
			carot.move_right();
		}
		else {
			carot.move(selection_box.get_last());
			selection_box.clear();
		}
	}
	else if (scancode == sf::Keyboard::Scancode::Down) {
		if (!selection_box.is_active()) {
			carot.move_down();
		}
		else {
			carot.move(selection_box.get_last());
			carot.move_down();
			selection_box.clear();
		}
	}
	else if (scancode == sf::Keyboard::Scancode::Up) {
		if (!selection_box.is_active()) {
			carot.move_up();
		}
		else {
			carot.move(selection_box.get_first());
			carot.move_up();
			selection_box.clear();
		}
	}
}

void Screen::type_char(int unicode) {

	const int backspace_unicode = 8;
	const int delete_unicode = 127;

	if (unicode == backspace_unicode) {

		if (selection_box.is_active()) {
			delete_selection();
			return;
		}

		if (carot.get_pos() <= 0) {
			return;
		}

		buffer.remove(carot.get_pos());

		text.setString(buffer.get_display_str());
		carot.move_left();
	}
	else if (unicode == delete_unicode) {

		if (selection_box.is_active()) {
			delete_selection();
			return;
		}

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

		if (selection_box.is_active()) {
			delete_selection();
		}

		buffer.insert(c, carot.get_pos());

		text.setString(buffer.get_display_str());
		carot.move_right();
	}
}

void Screen::delete_selection() {

	carot.move(selection_box.get_first());

	// repeatedly remove the first character in the selection from the buffer. Since the element is
	// removed, each loop iteration removes the next character in order.
	int selection_length = selection_box.get_last() - selection_box.get_first();
	for (int i = 0; i < selection_length; i++) {
		buffer.remove(selection_box.get_first() + 1);
	}
	selection_box.clear();

	text.setString(buffer.get_display_str());
}

int Screen::pos_to_char_index(sf::Vector2i screen_pos) {

	sf::Vector2f world_pos = window.mapPixelToCoords(screen_pos, view_handler.get_text_view());

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
	int char_index_in_line = round(((float)world_pos.x) / CHARACTER_WIDTH);
	char_index_in_line = clamp(char_index_in_line, 0, chars_in_line);

	return line_start_index + char_index_in_line;
}
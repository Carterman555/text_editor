#include <iostream>

#include "screen.hpp"
#include "constants.hpp"

using namespace Constants;

Screen::Screen() : buffer(),
font((std::string)PROJECT_DIR + "/CONSOLA.TTF"),
text(font),
caret(text),
selection_box(text),
window(sf::VideoMode(WINDOW_SIZE), "Text Editor"),
scroll_view(window) {

	window.setPosition({ 50, 50 });

	text.setString("");
	text.setCharacterSize(FONT_SIZE);
	text.setFillColor(sf::Color::White);

	caret.set_on_move([this](int pos) {ensure_caret_visible(pos);});

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

Screen::~Screen() {
}

void Screen::handle_events() {

	while (const std::optional event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			window.close();
		}
		else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
			scroll_view.handle_window_resize(resized->size);
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
			on_key_pressed(keyPressed->scancode);
		}
		else if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
			type_char(textEntered->unicode);
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
			scroll_view.scroll_by_delta(Axis::Y, mouseData->delta);
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
		int selection_start = selection_box.is_active() ? selection_box.get_start() : caret.get_pos();
		caret.move_left();
		selection_box.create(selection_start, caret.get_pos());
	}
	else if (shift && scancode == sf::Keyboard::Scancode::Right) {
		int selection_start = selection_box.is_active() ? selection_box.get_start() : caret.get_pos();
		caret.move_right();
		selection_box.create(selection_start, caret.get_pos());
	}
	else if (shift && scancode == sf::Keyboard::Scancode::Up) {
		int selection_start = selection_box.is_active() ? selection_box.get_start() : caret.get_pos();
		caret.move_up();
		selection_box.create(selection_start, caret.get_pos());
	}
	else if (shift && scancode == sf::Keyboard::Scancode::Down) {
		int selection_start = selection_box.is_active() ? selection_box.get_start() : caret.get_pos();
		caret.move_down();
		selection_box.create(selection_start, caret.get_pos());
	}
	else if (scancode == sf::Keyboard::Scancode::Left) {
		if (!selection_box.is_active()) {
			caret.move_left();
		}
		else {
			caret.move(selection_box.get_first());
			selection_box.clear();
		}
	}
	else if (scancode == sf::Keyboard::Scancode::Right) {
		if (!selection_box.is_active()) {
			caret.move_right();
		}
		else {
			caret.move(selection_box.get_last());
			selection_box.clear();
		}
	}
	else if (scancode == sf::Keyboard::Scancode::Down) {
		if (!selection_box.is_active()) {
			caret.move_down();
		}
		else {
			caret.move(selection_box.get_last());
			caret.move_down();
			selection_box.clear();
		}
	}
	else if (scancode == sf::Keyboard::Scancode::Up) {
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

void Screen::type_char(int unicode) {

	const int backspace_unicode = 8;
	const int delete_unicode = 127;

	if (unicode == backspace_unicode) {

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
	else if (unicode == delete_unicode) {

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
	else if (unicode < 128) {
		char c = static_cast<char>(unicode);

		const int return_unicode = 13;
		if (unicode == return_unicode) {
			c = '\n';
		}

		if (selection_box.is_active()) {
			delete_selection();
		}

		buffer.insert(c, caret.get_pos());

		update_text();
		caret.move_right();
	}
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
	// calculate the index

	//... 0 for the first char of each line, 1 for the second, and so on
	int char_index_in_line = round(((float)world_pos.x) / CHARACTER_WIDTH);
	char_index_in_line = clamp(char_index_in_line, 0, chars_in_line);

	return line_start_index + char_index_in_line;
}
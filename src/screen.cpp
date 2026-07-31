#include <iostream>

#include "screen.hpp"
#include "filehandler.hpp"
#include "constants.hpp"

using namespace Constants;

Screen::Screen(const string& contents) : buffer(),
font((std::string)PROJECT_DIR + "/CONSOLA.TTF"),
text(font),
caret(buffer),
selection_box(buffer),
window(sf::VideoMode(WINDOW_SIZE), "Text Editor"),
scroll_view(window) {

	window.setPosition({ 50, 50 });

	text.setCharacterSize(FONT_SIZE);
	text.setFillColor(sf::Color::White);

	for (int i = 0; i < contents.size(); i++) {
		buffer.insert(contents.at(i), i);
	}
	update_text();

	caret.set_on_move([this](sf::Vector2f char_pos) {ensure_caret_visible(char_pos);});
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
			if (const auto c = get_valid_char(textEntered->unicode)) {
				type_char(*c);
			}
		}
		else if (const auto* mouseData = event->getIf<sf::Event::MouseMoved>()) {
			bool drag_scrolling = scroll_view.get_scroll_bar(Axis::X).is_dragging() || scroll_view.get_scroll_bar(Axis::Y).is_dragging();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !drag_scrolling) {

				sf::Vector2f mouse_pos = window.mapPixelToCoords(mouseData->position, scroll_view.get_content_view());

				int desired_caret_pos = Helpers::pos_to_char_index(buffer.get_display_str(), mouse_pos);
				sf::Vector2f char_pos = caret.move(desired_caret_pos);

				selection_box.set_position(selection_start_index, caret.get_pos());
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
					sf::Vector2f mouse_pos = window.mapPixelToCoords(mouseData->position, scroll_view.get_content_view());
					int desired_caret_pos = Helpers::pos_to_char_index(buffer.get_display_str(), mouse_pos);
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

		if (caret.get_pos() >= buffer.get_display_str().size()) {
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
			int unicode = clipboard.at(i);
			if (auto c = get_valid_char(unicode)) {
				buffer.insert(*c, caret.get_pos() + i);
			}
			else {
				buffer.insert(U'\uFFFD', caret.get_pos() + i);
			}
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
		selection_box.set_position(selection_start, caret.get_pos());
	}
	else if (keyPressed->shift && keyPressed->scancode == sf::Keyboard::Scancode::Right) {
		int selection_start = selection_box.is_active() ? selection_box.get_start() : caret.get_pos();
		caret.move_right();
		selection_box.set_position(selection_start, caret.get_pos());
	}
	else if (keyPressed->shift && keyPressed->scancode == sf::Keyboard::Scancode::Up) {
		int selection_start = selection_box.is_active() ? selection_box.get_start() : caret.get_pos();
		caret.move_up();
		selection_box.set_position(selection_start, caret.get_pos());
	}
	else if (keyPressed->shift && keyPressed->scancode == sf::Keyboard::Scancode::Down) {
		int selection_start = selection_box.is_active() ? selection_box.get_start() : caret.get_pos();
		caret.move_down();
		selection_box.set_position(selection_start, caret.get_pos());
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

void Screen::type_char(char32_t c) {

	if (selection_box.is_active()) {
		delete_selection();
	}

	buffer.insert(c, caret.get_pos());

	update_text();
	caret.move_right();
}

const std::optional<char32_t> Screen::get_valid_char(int unicode) const {
	if (unicode == 9 || unicode == 10 || (unicode >= 32 && unicode < 127)) {
		return (char32_t)unicode;
	}
	else if (unicode == 13) {
		return '\n';
	}
	else {
		return std::nullopt;
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


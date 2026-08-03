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
scroll_view(window),
event_manager(window) {

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

	vector<Event> events = event_manager.poll_events();

	for (Event event : events) {
		if (holds_alternative<WindowClosed>(event) || holds_alternative<Escape>(event)) {
			window.close();
		}
		else if (holds_alternative<WindowResized>(event)) {
			scroll_view.handle_window_resize();
		}
		else if (holds_alternative<TypeChar>(event)) {
			type_char(get<TypeChar>(event).c);
		}
		else if (holds_alternative<MouseMoved>(event)) {
			bool drag_scrolling = scroll_view.get_scroll_bar(Axis::X).is_dragging() || scroll_view.get_scroll_bar(Axis::Y).is_dragging();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !drag_scrolling) {

				sf::Vector2f mouse_pos = window.mapPixelToCoords(get<MouseMoved>(event).position, scroll_view.get_content_view());

				int desired_caret_pos = Helpers::pos_to_char_index(buffer.get_display_str(), mouse_pos);
				sf::Vector2f char_pos = caret.move(desired_caret_pos);

				selection_box.set_position(selection_start_index, caret.get_pos());
			}
		}
		else if (holds_alternative<LeftClick>(event)) {
			if (scroll_view.get_scroll_bar(Axis::X).mouse_in_scroll_area()) {
				scroll_view.get_scroll_bar(Axis::X).start_dragging_scroll_bar();
			}
			else if (scroll_view.get_scroll_bar(Axis::Y).mouse_in_scroll_area()) {
				scroll_view.get_scroll_bar(Axis::Y).start_dragging_scroll_bar();
			}
		}
		else if (holds_alternative<SingleLeftClick>(event)) {
			sf::Vector2f mouse_pos = window.mapPixelToCoords(get<SingleLeftClick>(event).position, scroll_view.get_content_view());
			int desired_caret_pos = Helpers::pos_to_char_index(buffer.get_display_str(), mouse_pos);

			caret.move(desired_caret_pos);

			selection_start_index = caret.get_pos();
			selection_box.clear();
		}
		else if (holds_alternative<DoubleLeftClick>(event)) {
		}
		else if (holds_alternative<TripleLeftClick>(event)) {
		}
		else if (holds_alternative<LeftReleased>(event)) {
			scroll_view.get_scroll_bar(Axis::X).stop_dragging_scroll_bar();
			scroll_view.get_scroll_bar(Axis::Y).stop_dragging_scroll_bar();
		}
		else if (holds_alternative<MouseWheelScrolled>(event)) {
			float delta = get<MouseWheelScrolled>(event).delta;
			if (get<MouseWheelScrolled>(event).ctrl) {
				if (delta < 0) {
					scroll_view.zoom_out();
				}
				else if (delta > 0) {
					scroll_view.zoom_in();
				}
			}
			else {
				scroll_view.scroll_by_delta(Axis::Y, delta);
			}
		}
		else if (holds_alternative<Backspace>(event)) {
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
		else if (holds_alternative<Delete>(event)) {
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

		handle_commands(event);
		handle_arrow_keys(event);
	}
}

void Screen::handle_commands(const Event& event) {

	if (holds_alternative<CtrlS>(event)) {
		if (on_save) on_save(buffer.get_display_str());
	}
	else if (holds_alternative<CtrlC>(event)) {
		if (selection_box.is_active()) {
			sf::Clipboard::setString(selection_box.get_selection());
		}
	}
	else if (holds_alternative<CtrlV>(event)) {
		if (selection_box.is_active()) {
			delete_selection();
		}

		string clipboard = sf::Clipboard::getString();
		for (int i = 0; i < clipboard.size(); i++) {
			int unicode = clipboard.at(i);
			if (auto c = Helpers::get_valid_char(unicode)) {
				buffer.insert(*c, caret.get_pos() + i);
			}
			else {
				buffer.insert(U'\uFFFD', caret.get_pos() + i);
			}
		}

		update_text();

		caret.move(caret.get_pos() + clipboard.size());
	}
	else if (holds_alternative<CtrlA>(event)) {
		// select all text
		selection_box.set_position(0, buffer.get_display_str().size());
	}
	else if (holds_alternative<CtrlHyphen>(event)) {
		scroll_view.zoom_out();
	}
	else if (holds_alternative<CtrlEqual>(event)) {
		scroll_view.zoom_in();
	}
}

void Screen::handle_arrow_keys(const Event& event) {

	if (!holds_alternative<ArrowKey>(event)) {
		return;
	}

	sf::Keyboard::Scancode scancode = get<ArrowKey>(event).scancode;

	if (get<ArrowKey>(event).shift) {
		int selection_start = selection_box.is_active() ? selection_box.get_start() : caret.get_pos();

		switch (scancode) {
			case sf::Keyboard::Scancode::Left: caret.move_left(); break;
			case sf::Keyboard::Scancode::Right: caret.move_right(); break;
			case sf::Keyboard::Scancode::Up: caret.move_up(); break;
			case sf::Keyboard::Scancode::Down: caret.move_down(); break;
		}

		selection_box.set_position(selection_start, caret.get_pos());
	}
	else {
		if (!selection_box.is_active()) {
			switch (scancode) {
				case sf::Keyboard::Scancode::Left: caret.move_left(); break;
				case sf::Keyboard::Scancode::Right: caret.move_right(); break;
				case sf::Keyboard::Scancode::Up: caret.move_up(); break;
				case sf::Keyboard::Scancode::Down: caret.move_down(); break;
			}
		}
		else {
			switch (scancode) {
				case sf::Keyboard::Scancode::Left:
					caret.move(selection_box.get_first());
					break;
				case sf::Keyboard::Scancode::Right:
					caret.move(selection_box.get_last());
					break;
				case sf::Keyboard::Scancode::Up:
					caret.move(selection_box.get_first());
					caret.move_up();
					break;
				case sf::Keyboard::Scancode::Down:
					caret.move(selection_box.get_last());
					caret.move_down();
					break;
			}
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


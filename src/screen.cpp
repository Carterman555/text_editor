#include <iostream>
#include <typeinfo>

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

	if (contains_non_ascii(contents)) {
		// tinyfd_messageBox("Warning", "message", "ok", "warning", 1);
	}

	type_sequence(contents);

	caret.move(0);
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

bool Screen::contains_non_ascii(const string& str) {
	for (char c : str) {
		if (!Helpers::filter_char(c).has_value()) {
			return true;
		}
	}
	return false;
}

void Screen::handle_events() {

	vector<Event> events = event_manager.poll_events();

	bool mouse_in_text_area = !scroll_view.get_scroll_bar(Axis::X).mouse_in_scroll_area() &&
		!scroll_view.get_scroll_bar(Axis::Y).mouse_in_scroll_area();

	for (Event event : events) {
		if (holds_alternative<WindowClosed>(event) || holds_alternative<Escape>(event)) {
			window.close();
		}
		else if (holds_alternative<WindowResized>(event)) {

			sf::Vector2u new_window_size = window.getSize();

			new_window_size.x = max(new_window_size.x, MIN_WINDOW_SIZE.x);
			new_window_size.y = max(new_window_size.y, MIN_WINDOW_SIZE.y);

			window.setSize(new_window_size);

			scroll_view.handle_window_resize();
		}
		else if (holds_alternative<TypeChar>(event)) {
			type_char(get<TypeChar>(event).c);
		}
		else if (holds_alternative<MouseMoved>(event)) {
			bool drag_scrolling = scroll_view.get_scroll_bar(Axis::X).is_dragging() || scroll_view.get_scroll_bar(Axis::Y).is_dragging();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !drag_scrolling) {

				int pos = mouse_text_pos(get<MouseMoved>(event).position);
				sf::Vector2f char_pos = caret.move(pos);

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
			if (mouse_in_text_area) {
				int pos = mouse_text_pos(get<SingleLeftClick>(event).position);
				caret.move(pos);

				selection_start_index = caret.get_pos();
				selection_box.clear();
			}
		}
		else if (holds_alternative<DoubleLeftClick>(event)) {
			if (mouse_in_text_area) {
				int pos = mouse_text_pos(get<DoubleLeftClick>(event).position);
				select_group(pos);
			}
		}
		else if (holds_alternative<TripleLeftClick>(event)) {
			if (mouse_in_text_area) {
				int pos = mouse_text_pos(get<TripleLeftClick>(event).position);
				select_line(pos);
			}
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
			}
			else if (caret.get_pos() > 0) {
				buffer.remove(caret.get_pos());

				update_text();
				caret.move_left();
			}
		}
		else if (holds_alternative<Delete>(event)) {
			if (selection_box.is_active()) {
				delete_selection();
			}
			else if (caret.get_pos() < buffer.get_display_str().size()) {
				buffer.remove(caret.get_pos() + 1);

				update_text();
			}
		}

		handle_commands(event);
		handle_arrow_keys(event);
	}
}

void Screen::select_group(int pos) {

	const string& text = buffer.get_display_str();

	bool caret_at_end = pos >= text.length();
	if (caret_at_end) {
		selection_box.clear();
		return;
	}

	// the character after the caret determines the group to select
	char char_after_caret = text.at(pos);

	unordered_set<char> letters_and_numbers = {
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
		'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
		'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
		'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
		'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
		'Y', 'Z', '_', '0', '1', '2', '3', '4', '5', '6',
		'7', '8', '9'
	};

	unordered_set<char> symbols = {
		'.', ',', ';', ':', '!', '?', '"', '\'', '(', ')', '[', ']', '{', '}',
		'<', '>', '/', '\\', '|', '+', '-', '=', '*', '&', '%', '^', '~', '@',
		'#', '$', '`'
	};

	unordered_set<char> whitespace = {
		' ', '\t'
	};

	unordered_set<char> char_set;
	if (letters_and_numbers.contains(char_after_caret)) char_set = letters_and_numbers;
	else if (symbols.contains(char_after_caret)) char_set = symbols;
	else if (whitespace.contains(char_after_caret)) char_set = whitespace;
	else char_set = symbols;

	// find the index of the start and end of the selection
	int start = pos;
	while (start > 0 && char_set.contains(text.at(start - 1))) {
		start--;
	}

	int end = pos;
	while (end < text.length() && char_set.contains(text.at(end))) {
		end++;
	}

	start = max(start, 0);
	end = min(end, (int)text.length());

	selection_box.set_position(start, end);
	caret.move(end);
}

void Screen::select_line(int pos) {
	const string& text = buffer.get_display_str();

	int start = pos;
	while (start > 0 && text.at(start - 1) != '\n') {
		start--;
	}

	int end = pos;
	while (end < text.length() && text.at(end) != '\n') {
		end++;
	}

	selection_box.set_position(start, end);
	caret.move(end);
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

		string clipboard = sf::Clipboard::getString().toAnsiString();
		type_sequence(clipboard);
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

void Screen::type_char(char c) {

	if (selection_box.is_active()) {
		delete_selection();
	}

	buffer.insert(c, caret.get_pos());

	update_text();
	caret.move_right();
}

void Screen::type_sequence(const string& str) {
	int inserted_count = 0;
	for (int i = 0; i < str.size(); i++) {

		// Windows uses '/r/n' for returns. `get_valid_char()` turns each of these into '\n'
		// it should only insert one '\n'. To prevent this, check if the '/r/n' sequence
		// occurs and skip the first '\n' if it does, so it only inserts one into the buffer.
		bool this_char_is_CR = str.at(i) == 13;
		bool next_char_is_LF = i + 1 < str.size() && str.at(i + 1) == 10;
		if (this_char_is_CR && next_char_is_LF) {
			continue;
		}

		if (auto c = Helpers::filter_char(str.at(i))) {
			buffer.insert(str.at(i), caret.get_pos() + inserted_count);
			inserted_count++;
		}
	}

	update_text();
	caret.move(caret.get_pos() + inserted_count);
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
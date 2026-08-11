
#include "helpers.hpp"

#include <cmath>
#include <iostream>
#include <sstream>
#include <algorithm>

#include "constants.hpp"

using namespace Constants;

int Helpers::pos_to_char_index(const std::string& text, sf::Vector2f world_pos) {

	// Step 1: Get the line number, the start index of that line, and the size of the line
	double target_line_d = std::max<double>(ceil(world_pos.y / LINE_HEIGHT), 1);
	std::size_t target_line = static_cast<std::size_t>(target_line_d);

	std::size_t cur_line = 1;

	std::size_t line_start_index = 0;
	std::size_t line_end_index = 0;

	while (cur_line < target_line) {
		line_end_index = text.find('\n', line_start_index);

		// If the line number was not reached this means the cursor position was below the last line.
		// When this happens, it always move the caret to after the last character
		bool no_more_newlines = line_end_index == std::string::npos;
		if (no_more_newlines) {
			return text.size();
		}

		cur_line++;
		line_start_index = line_end_index + 1;
	}


	line_end_index = text.find('\n', line_start_index);
	bool last_line = line_end_index == std::string::npos;
	if (last_line) {
		line_end_index = text.size();
	}

	std::size_t line_size = line_end_index - line_start_index;

	// Step 2: Use the start index of that line and the size of the line to
	// calculate the index. Go through each char in the line until it reaches the world x to
	// get the char_index_in_line.

	//... 0 for the first char of each line, 1 for the second, and so on
	int char_index_in_line;

	int current_x = 0;
	for (char_index_in_line = 0; char_index_in_line < line_size; char_index_in_line++) {
		int char_index = line_start_index + char_index_in_line;

		int cur_char_width = text.at(char_index) == '\t' ? TAB_WIDTH : CHARACTER_WIDTH;
		current_x += cur_char_width;

		if (current_x > world_pos.x + (cur_char_width / 2.f)) {
			break;
		}
	}

	return line_start_index + char_index_in_line;
}

sf::Vector2f Helpers::char_index_to_pos(const std::string& text, std::size_t index) {

	index = std::min(text.size(), index);

	sf::Vector2f world_pos = { 0, 0 };

	for (std::size_t i = 0; i < index; i++) {
		if (text.at(i) == '\n') {
			world_pos.x = 0;
			world_pos.y += LINE_HEIGHT;
		}
		else if (text.at(i) == '\t') {
			world_pos.x += TAB_WIDTH;
		}
		else {
			world_pos.x += CHARACTER_WIDTH;
		}
	}

	return world_pos;
}

sf::Vector2f Helpers::find_text_area_size(const std::string& text) {
	sf::Vector2f size = { 0, LINE_HEIGHT };

	float current_width = 0;

	for (size_t i = 0; i < text.size(); i++) {
		if (text.at(i) == '\n') {
			size.y += LINE_HEIGHT;
			current_width = 0;
		}
		else if (text.at(i) == '\t') {
			current_width += TAB_WIDTH;
		}
		else {
			current_width += CHARACTER_WIDTH;
		}

		if (current_width > size.x) {
			size.x = current_width;
		}
	}

	return size;
}

const std::optional<char> Helpers::filter_char(char c) {
	if (c == 9 || c == 10 || (c >= 32 && c < 127)) {
		return c;
	}
	else if (c == 13) {
		return '\n';
	}
	else {
		return std::nullopt;
	}
}
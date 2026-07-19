
#include "helpers.hpp"

#include <bits/c++io.h>
#include <cmath>

#include "constants.hpp"

using namespace Constants;
using namespace std;

int Helpers::pos_to_char_index(const std::string& text, sf::Vector2f world_pos) {

	// Step 1: Get the line number, the start index of that line, and the size of the line
	int line_number = ceil(world_pos.y / LINE_HEIGHT);

	if (line_number < 1) {
		line_number = 1;
	}

	int line_counter = 0;

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

sf::Vector2f Helpers::char_index_to_pos(const std::string& text, ulong index) {

	index = min(text.size(), index);

	sf::Vector2f world_pos = { 0, 0 };

	for (ulong i = 0; i < index; i++) {
		if (text.at(i) == '\t') {
			world_pos.x += TAB_WIDTH;
		}
		else if (text.at(i) == '\n') {
			world_pos.x = 0;
			world_pos.y += LINE_HEIGHT;
		}
		else {
			world_pos.x += CHARACTER_WIDTH;
		}
	}

	return world_pos;
}


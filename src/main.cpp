#include <iostream>

#include "screen.hpp"
#include "filehandler.hpp"

int main(int argc, char* argv[]) {

	std::string contents = "";
	if (argc == 2) {
		if (auto text = FileHandler::read_text_file(argv[1])) {
			contents = *text;
		}
		else {
			std::cerr << "Error: failed to read text file (" << argv[1] << ")!" << std::endl;
			return 1;
		}
	}
	if (argc > 2) {
		std::cerr << "Error: program received too many parameters" << std::endl;
		return 1;
	}

	Screen screen(contents);

	if (argc == 2) {
		screen.set_on_save([argv](const std::string& contents) {
			FileHandler::write_text_file(argv[1], contents);
			});
	}

	screen.run_window();
}



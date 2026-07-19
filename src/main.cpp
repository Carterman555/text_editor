#include <iostream>

#include "screen.hpp"
#include "filehandler.hpp"

using namespace std;

int main(int argc, char* argv[]) {

	string contents = "";
	if (argc == 2) {
		if (auto text = FileHandler::read_text_file(argv[1])) {
			contents = *text;
		}
		else {
			cerr << "Error: failed to read text file (" << argv[1] << ")!" << endl;
			return 1;
		}
	}
	if (argc > 2) {
		cerr << "Error: program received too many parameters" << endl;
		return 1;
	}

	Screen screen(contents);

	if (argc == 2) {
		screen.set_on_save([argv](const string& contents) {
			FileHandler::write_text_file(argv[1], contents);
			});
	}

	screen.run_window();
}



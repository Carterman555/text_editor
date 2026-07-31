#include <fstream>
#include <sstream>
#include <iostream>
#include <optional>

#include "filehandler.hpp"

std::optional<string> FileHandler::read_text_file(const string& file_path) {

    ifstream file(file_path);

    if (!file.is_open()) {
        return std::nullopt;
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    std::string contents = ss.str();

    return contents;
}

bool FileHandler::write_text_file(const string& file_path, const string& text) {
    fstream file(file_path, ios::out);

    if (!file.is_open()) {
        cerr << "Error: Failed to open file (" << file_path << ")" << endl;
        return false;
    }

    file << text;

    file.close();

    return true;
}
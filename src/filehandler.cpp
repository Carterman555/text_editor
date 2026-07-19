#include <fstream>
#include <iostream>
#include <optional>

#include "filehandler.hpp"

std::optional<string> FileHandler::read_text_file(string file_path) {

    ifstream file(file_path);

    if (!file.is_open()) {
        cerr << "Error: Failed to open file (" << file_path << ")" << endl;
        return std::nullopt;
    }

    string contents;
    string line;
    while (getline(file, line)) {
        contents += line + "\n";
    }

    if (!file.eof()) {
        cerr << "Error: File reading failed (" << file_path << ")" << endl;
        file.close();
        return std::nullopt;
    }

    file.close();

    return contents;
}

bool FileHandler::write_text_file(string file_path, const string& text) {
    fstream file(file_path, ios::out);

    if (!file.is_open()) {
        cerr << "Error: Failed to open file (" << file_path << ")" << endl;
        return false;
    }

    file << text;

    file.close();

    return true;
}
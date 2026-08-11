#include <fstream>
#include <sstream>
#include <iostream>

#include "filehandler.hpp"

std::optional<std::string> FileHandler::read_text_file(const std::string& file_path) {

    std::ifstream file(file_path);

    if (!file.is_open()) {
        return std::nullopt;
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    std::string contents = ss.str();

    return contents;
}

bool FileHandler::write_text_file(const std::string& file_path, const std::string& text) {
    std::fstream file(file_path, std::ios::out);

    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file (" << file_path << ")" << std::endl;
        return false;
    }

    file << text;

    file.close();

    return true;
}
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

bool FileHandler::write_text_file(const std::filesystem::path& file_path, const std::string& text) {

    std::filesystem::path dir_path = file_path.parent_path();

    if (dir_path.empty()) {
        dir_path = std::filesystem::path(".");
    }

    // write to a temp file to make sure content is successfully written before overriding original
    // text
    std::filesystem::path tmp_path = dir_path / (file_path.filename().string() + "-temp.txt");
    std::fstream tmp_file(tmp_path, std::ios::out);
    tmp_file << text;
    tmp_file.close();

    if (!tmp_file) {
        std::filesystem::remove(tmp_path);
        std::cerr << "Error: Failed to write to file (" << file_path << ")" << std::endl;
        return false;
    }

    std::error_code rename_ec;
    std::filesystem::rename(tmp_path, file_path, rename_ec);
    if (rename_ec) {
        std::error_code remove_ec;
        std::filesystem::remove(tmp_path, remove_ec);
        std::cerr << "Error: Failed to write to file (" << file_path << ")" << std::endl;
        return false;
    }

    return true;
}
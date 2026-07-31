#pragma once

#include <string>

using namespace std;

class FileHandler {
public:
    // Get the contents of a text file. Returns nullopt if fails.
    static std::optional<string> read_text_file(const string& file_path);

    /**
     * @brief replace the contents of a text file
     *
     * @return whether successfully wrote to file
     */
    static bool write_text_file(const string& file_path, const string& text);
};
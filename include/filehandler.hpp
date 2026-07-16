#pragma once

#include <string>

using namespace std;

class FileHandler {
public:
    // Fet the contents of a text file. Returns nullopt if fails.
    static std::optional<string> read_text_file(string file_path);

    /**
     * @brief replace the contents of a text file
     *
     * @return whether successfully wrote to file
     */
    static bool write_text_file(string file_path, string text);
};
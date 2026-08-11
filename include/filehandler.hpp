#pragma once

#include <string>
#include <optional>

class FileHandler {
public:
    // Get the contents of a text file. Returns nullopt if fails.
    static std::optional<std::string> read_text_file(const std::string& file_path);

    /**
     * @brief replace the contents of a text file
     *
     * @return whether successfully wrote to file
     */
    static bool write_text_file(const std::string& file_path, const std::string& text);
};
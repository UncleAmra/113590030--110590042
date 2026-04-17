#ifndef UTIL_LOAD_TEXT_FILE_HPP
#define UTIL_LOAD_TEXT_FILE_HPP
#include <fstream>
#include "Util/Logger.hpp"
#include <string>
#include <vector>
#include "pch.hpp" // IWYU pragma: export

namespace Util {
/**
 * @brief Generic helper function that loads a text file into a `std::string`.
 */
std::string LoadTextFile(const std::string &filepath);

// ADD THIS NEW FUNCTION FOR DIALOGUE!
    inline std::vector<std::string> LoadDialogueFile(const std::string& filepath) {
        std::vector<std::string> lines;
        std::ifstream file(filepath);
        
        if (!file.is_open()) {
            LOG_ERROR("Failed to load dialogue: " + filepath);
            lines.push_back("..."); 
            return lines;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            if (!line.empty()) {
                lines.push_back(line);
            }
        }
        return lines;
    }
} // namespace Util

#endif
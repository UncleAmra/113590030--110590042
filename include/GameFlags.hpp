#pragma once
#include <string>
#include <unordered_map>

namespace GameFlags {
    // This dictionary holds all of our story variables
    inline std::unordered_map<std::string, bool> Flags;

    // Helper to check if a flag is true (defaults to false if it doesn't exist)
    inline bool Get(const std::string& key) {
        return Flags[key]; 
    }

    // Helper to set a flag
    inline void Set(const std::string& key, bool value) {
        Flags[key] = value;
    }
}
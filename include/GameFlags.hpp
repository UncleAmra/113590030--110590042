#pragma once
#include <string>
#include <unordered_map>

namespace GameFlags {
    // This dictionary holds all of our story variables
    inline std::unordered_map<std::string, bool> Flags;

    // Helper to check if a flag is true (Safely checks without adding new entries)
    inline bool Get(const std::string& key) {
        auto it = Flags.find(key);
        if (it == Flags.end()) {
            return false; // Key wasn't found, so the "event" hasn't happened
        }
        return it->second; // Return the actual stored value
    }

    // Helper to set a flag
    inline void Set(const std::string& key, bool value) {
        Flags[key] = value; // Here, [] is fine because we WANT to create/overwrite
    }
}
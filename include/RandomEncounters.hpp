#ifndef RandomEncounters_HPP
#define RandomEncounters_HPP
#include <vector>
#include "GameConfig.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <unordered_map>
#include <string>

namespace RandomEncounters {
    struct WildEncounterEntry {
        std::string speciesName; 
        int minLevel;
        int maxLevel;
        int weight; // Higher weight = more common (e.g., 50 for common, 5 for rare)
    };

    // Map a mapPath to a specific list of encounters!
    inline std::unordered_map<std::string, std::vector<WildEncounterEntry>> MapEncounters = {
        
        // Town Outskirts Encounters
        { RESOURCE_DIR "/maps/level", {
            { "Starly", 2, 4, 50 },    // Very common
            { "Rattata", 2, 4, 40 },   // Common
            { "Pikachu", 3, 5, 10 },
            { "PaldeanWooper", 3, 5, 50 }   // Rare
        }},

        // Forest Maze Encounters
        { RESOURCE_DIR "/maps/maze", {
            { "Bulbasaur", 4, 6, 20 }, // Uncommon
            { "Starly", 3, 5, 80 }     // Very common
        }}
    };
}
#endif
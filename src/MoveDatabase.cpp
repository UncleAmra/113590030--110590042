#include "MoveDatabase.hpp"
#include "Util/Logger.hpp" // Assuming you want to log errors
#include <stdexcept>

// Initialize the static map
std::unordered_map<std::string, MoveData> MoveDatabase::s_Moves;

void MoveDatabase::Init() {
    // Normal Moves
    s_Moves["Tackle"]  = { "Tackle",  PokemonType::NORMAL, MoveCategory::PHYSICAL, 40, 100, 35 };
    s_Moves["Scratch"] = { "Scratch", PokemonType::NORMAL, MoveCategory::PHYSICAL, 40, 100, 35 };
    s_Moves["Growl"]   = { "Growl",   PokemonType::NORMAL, MoveCategory::STATUS,    0, 100, 40 };
    
    // Elemental Starter Moves
    s_Moves["Ember"]     = { "Ember",     PokemonType::FIRE,  MoveCategory::SPECIAL,  40, 100, 25 };
    s_Moves["Water Gun"] = { "Water Gun", PokemonType::WATER, MoveCategory::SPECIAL,  40, 100, 25 };
    s_Moves["Vine Whip"] = { "Vine Whip", PokemonType::GRASS, MoveCategory::PHYSICAL, 45, 100, 25 };
    
    // Add as many as you want here!
}

const MoveData& MoveDatabase::GetMove(const std::string& moveName) {
    auto it = s_Moves.find(moveName);
    if (it != s_Moves.end()) {
        return it->second;
    }
    
    LOG_ERROR("Move not found in database: {}", moveName);
    // Fallback to avoid crashing, returning a safe dummy move
    static MoveData fallback = { "Struggle", PokemonType::NORMAL, MoveCategory::PHYSICAL, 50, 100, 1 };
    return fallback;
}

bool MoveDatabase::HasMove(const std::string& moveName) {
    return s_Moves.find(moveName) != s_Moves.end();
}
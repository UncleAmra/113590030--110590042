#include "MoveDatabase.hpp"
#include "Util/Logger.hpp"

std::unordered_map<std::string, MoveData> MoveDatabase::s_Moves;

void MoveDatabase::Init() {
    // Format: { name, type, category, power, accuracy, maxPP }
    using T = PokemonType;
    using C = MoveCategory;

    // --- NORMAL ---
    s_Moves["Tackle"]    = { "Tackle",    T::NORMAL,   C::PHYSICAL, 40,  95, 35 };
    s_Moves["Scratch"]   = { "Scratch",   T::NORMAL,   C::PHYSICAL, 40, 100, 35 };
    s_Moves["Quick Attack"] = { "Quick Attack", T::NORMAL, C::PHYSICAL, 40, 100, 30 };
    s_Moves["Hyper Fang"] = { "Hyper Fang", T::NORMAL, C::PHYSICAL, 80, 90, 15 };
    s_Moves["Growl"]     = { "Growl",     T::NORMAL,   C::STATUS,    0, 100, 40 };
    s_Moves["Tail Whip"] = { "Tail Whip", T::NORMAL,   C::STATUS,    0, 100, 30 };

    // --- FIRE ---
    s_Moves["Ember"]         = { "Ember",     T::FIRE, C::SPECIAL, 40, 100, 25 };
    s_Moves["Flamethrower"]  = { "Flamethrower", T::FIRE, C::SPECIAL, 90, 100, 15 };
    s_Moves["Fire Blast"]    = { "Fire Blast", T::FIRE, C::SPECIAL, 110, 85, 5 };

    // --- WATER ---
    s_Moves["Water Gun"]  = { "Water Gun",  T::WATER, C::SPECIAL, 40, 100, 25 };
    s_Moves["Surf"]       = { "Surf",       T::WATER, C::SPECIAL, 90, 100, 15 };
    s_Moves["Bubble"]     = { "Bubble",     T::WATER, C::SPECIAL, 40, 100, 30 };

    // --- GRASS ---
    s_Moves["Vine Whip"]  = { "Vine Whip",  T::GRASS, C::PHYSICAL, 45, 100, 25 };
    s_Moves["Razor Leaf"] = { "Razor Leaf", T::GRASS, C::PHYSICAL, 55,  95, 25 };
    s_Moves["Solar Beam"] = { "Solar Beam", T::GRASS, C::SPECIAL, 120, 100, 10 };

    // --- ELECTRIC ---
    s_Moves["Thunder Shock"] = { "Thunder Shock", T::ELECTRIC, C::SPECIAL, 40, 100, 30 };
    s_Moves["Thunderbolt"]   = { "Thunderbolt",   T::ELECTRIC, C::SPECIAL, 90, 100, 15 };

    // --- PSYCHIC ---
    s_Moves["Confusion"]  = { "Confusion", T::PSYCHIC, C::SPECIAL, 50, 100, 25 };
    s_Moves["Psychic"]    = { "Psychic",   T::PSYCHIC, C::SPECIAL, 90, 100, 10 };

    // --- FIGHTING ---
    s_Moves["Karate Chop"] = { "Karate Chop", T::FIGHTING, C::PHYSICAL, 50, 100, 25 };
    s_Moves["Low Kick"]    = { "Low Kick",    T::FIGHTING, C::PHYSICAL, 50, 90,  20 };

    // --- ROCK ---
    s_Moves["Rock Throw"] = { "Rock Throw", T::ROCK, C::PHYSICAL, 50, 90, 15 };
    s_Moves["Rock Slide"] = { "Rock Slide", T::ROCK, C::PHYSICAL, 75, 90, 10 };

    LOG_TRACE("MoveDatabase initialised with {} moves.", s_Moves.size());
}

//void MoveDatabase::Init() {}  // Remove this duplicate if present

const MoveData& MoveDatabase::GetMove(const std::string& moveName) {
    auto it = s_Moves.find(moveName);
    if (it == s_Moves.end()) {
        LOG_ERROR("Move '{}' not found!", moveName);
        static MoveData empty = { "Unknown", PokemonType::NORMAL,
                                   MoveCategory::STATUS, 0, 0, 0 };
        return empty;
    }
    return it->second;
}

bool MoveDatabase::HasMove(const std::string& moveName) {
    return s_Moves.count(moveName) > 0;
}
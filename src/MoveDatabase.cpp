#include "MoveDatabase.hpp"
#include "Util/Logger.hpp"

std::unordered_map<std::string, MoveData> MoveDatabase::s_Moves;

void MoveDatabase::Init() {
    // Format: { name, type, category, power, accuracy, maxPP }
    using T = PokemonType;
    using C = MoveCategory;

    // --- NORMAL ---
    s_Moves["Tackle"]       = { "Tackle",       T::NORMAL,   C::PHYSICAL, 40,  100, 35 };
    s_Moves["Scratch"]      = { "Scratch",      T::NORMAL,   C::PHYSICAL, 40,  100, 35 };
    s_Moves["Quick Attack"] = { "Quick Attack", T::NORMAL,   C::PHYSICAL, 40,  100, 30 };
    s_Moves["Hyper Fang"]   = { "Hyper Fang",   T::NORMAL,   C::PHYSICAL, 80,   90, 15 };
    s_Moves["Growl"]        = { "Growl",        T::NORMAL,   C::STATUS,    0,  100, 40 };
    s_Moves["Tail Whip"]    = { "Tail Whip",    T::NORMAL,   C::STATUS,    0,  100, 30 };
    s_Moves["Smokescreen"]  = { "Smokescreen",  T::NORMAL,   C::STATUS,    0,  100, 20 };
    s_Moves["Rage"]         = { "Rage",         T::NORMAL,   C::PHYSICAL, 20,  100, 20 };
    s_Moves["Slash"]        = { "Slash",        T::NORMAL,   C::PHYSICAL, 70,  100, 20 };
    s_Moves["Sweet Scent"]  = { "Sweet Scent",  T::NORMAL,   C::STATUS,    0,  100, 20 };
    s_Moves["Disable"]      = { "Disable",      T::NORMAL,   C::STATUS,    0,  100, 20 };
    s_Moves["Recover"]      = { "Recover",      T::NORMAL,   C::STATUS,    0,    0, 10 };
    s_Moves["Screech"]      = { "Screech",      T::NORMAL,   C::STATUS,    0,   85, 40 };
    s_Moves["Wrap"]         = { "Wrap",         T::NORMAL,   C::PHYSICAL, 15,   90, 20 };
    s_Moves["Leer"]         = { "Leer",         T::NORMAL,   C::STATUS,    0,  100, 30 };
    s_Moves["Glare"]        = { "Glare",        T::NORMAL,   C::STATUS,    0,  100, 30 };
    s_Moves["Fury Attack"]  = { "Fury Attack",  T::NORMAL,   C::PHYSICAL, 15,   85, 20 };
    s_Moves["Super Fang"]   = { "Super Fang",   T::NORMAL,   C::PHYSICAL,  0,   90, 10 };
    s_Moves["Double-Edge"]  = { "Double-Edge",  T::NORMAL,   C::PHYSICAL, 120, 100, 15 };
    s_Moves["Explosion"]    = { "Explosion",    T::NORMAL,   C::PHYSICAL, 250, 100,  5 };
    s_Moves["Sword Dance"]  = { "Sword Dance",  T::NORMAL,   C::STATUS,    0,    0, 20 }; // Commonly "Swords Dance"
    s_Moves["Swift"]        = { "Swift",        T::NORMAL,   C::SPECIAL,  60,    0, 20 };
    s_Moves["Hyper Beam"]   = { "Hyper Beam",   T::NORMAL,   C::SPECIAL, 150,   90,  5 };
    s_Moves["Extreme Speed"]= { "Extreme Speed",T::NORMAL,   C::PHYSICAL, 80,  100,  5 };
    s_Moves["Metronome"]    = { "Metronome",    T::NORMAL,   C::STATUS,    0,    0, 10 };
    s_Moves["Flail"]        = { "Flail",        T::NORMAL,   C::PHYSICAL,  0,  100, 15 }; // Variable power

    // --- FIRE ---
    s_Moves["Ember"]        = { "Ember",        T::FIRE,     C::SPECIAL,  40,  100, 25 };
    s_Moves["Flamethrower"] = { "Flamethrower", T::FIRE,     C::SPECIAL,  90,  100, 15 };
    s_Moves["Fire Blast"]   = { "Fire Blast",   T::FIRE,     C::SPECIAL, 110,   85,  5 };
    s_Moves["Fire Spin"]    = { "Fire Spin",    T::FIRE,     C::SPECIAL,  35,   85, 15 };
    s_Moves["Flame Wheel"]  = { "Flame Wheel",  T::FIRE,     C::PHYSICAL, 60,  100, 25 };

    // --- WATER ---
    s_Moves["Water Gun"]    = { "Water Gun",    T::WATER,    C::SPECIAL,  40,  100, 25 };
    s_Moves["Surf"]         = { "Surf",         T::WATER,    C::SPECIAL,  90,  100, 15 };
    s_Moves["Bubble"]       = { "Bubble",       T::WATER,    C::SPECIAL,  40,  100, 30 };
    s_Moves["Withdraw"]     = { "Withdraw",     T::WATER,    C::STATUS,    0,  100, 40 };
    s_Moves["Hydro Pump"]   = { "Hydro Pump",   T::WATER,    C::SPECIAL, 110,   80,  5 };
    s_Moves["Rain Dance"]   = { "Rain Dance",   T::WATER,    C::STATUS,    0,    0,  5 };

    // --- GRASS ---
    s_Moves["Vine Whip"]    = { "Vine Whip",    T::GRASS,    C::PHYSICAL, 45,  100, 25 };
    s_Moves["Razor Leaf"]   = { "Razor Leaf",   T::GRASS,    C::PHYSICAL, 55,   95, 25 };
    s_Moves["Solar Beam"]   = { "Solar Beam",   T::GRASS,    C::SPECIAL, 120,  100, 10 };
    s_Moves["Leech Seed"]   = { "Leech Seed",   T::GRASS,    C::STATUS,    0,   90, 10 };
    s_Moves["Petal Dance"]  = { "Petal Dance",  T::GRASS,    C::SPECIAL, 120,  100, 10 };
    s_Moves["Stun Spore"]   = { "Stun Spore",   T::GRASS,    C::STATUS,    0,   75, 30 };
    s_Moves["Absorb"]       = { "Absorb",       T::GRASS,    C::SPECIAL,  20,  100, 25 };
    s_Moves["Synthesis"]    = { "Synthesis",    T::GRASS,    C::STATUS,    0,    0,  5 };

    // --- ELECTRIC ---
    s_Moves["Thunder Shock"]= { "Thunder Shock",T::ELECTRIC, C::SPECIAL,  40,  100, 30 };
    s_Moves["Thunderbolt"]  = { "Thunderbolt",  T::ELECTRIC, C::SPECIAL,  90,  100, 15 };

    // --- PSYCHIC ---
    s_Moves["Confusion"]    = { "Confusion",    T::PSYCHIC,  C::SPECIAL,  50,  100, 25 };
    s_Moves["Psychic"]      = { "Psychic",      T::PSYCHIC,  C::SPECIAL,  90,  100, 10 };
    s_Moves["Teleport"]     = { "Teleport",     T::PSYCHIC,  C::STATUS,    0,    0, 20 };
    s_Moves["Kinesis"]      = { "Kinesis",      T::PSYCHIC,  C::STATUS,    0,   80, 15 };
    s_Moves["Agility"]      = { "Agility",      T::PSYCHIC,  C::STATUS,    0,    0, 30 };
    s_Moves["Reflect"]      = { "Reflect",      T::PSYCHIC,  C::STATUS,    0,    0, 20 };

    // --- FIGHTING ---
    s_Moves["Karate Chop"]  = { "Karate Chop",  T::FIGHTING, C::PHYSICAL, 50,  100, 25 };
    s_Moves["Low Kick"]     = { "Low Kick",     T::FIGHTING, C::PHYSICAL, 50,   90, 20 }; // Power variable by weight

    // --- ROCK ---
    s_Moves["Rock Throw"]   = { "Rock Throw",   T::ROCK,     C::PHYSICAL, 50,   90, 15 };
    s_Moves["Rock Slide"]   = { "Rock Slide",   T::ROCK,     C::PHYSICAL, 75,   90, 10 };

    // --- POISON ---
    s_Moves["Poison Sting"] = { "Poison Sting", T::POISON,   C::PHYSICAL, 15,  100, 35 };
    s_Moves["Acid"]         = { "Acid",         T::POISON,   C::SPECIAL,  40,  100, 30 };

    // --- DARK ---
    s_Moves["Bite"]         = { "Bite",         T::DARK,     C::PHYSICAL, 60,  100, 25 };
    s_Moves["Crunch"]       = { "Crunch",       T::DARK,     C::PHYSICAL, 80,  100, 15 };

    // --- FLYING ---
    s_Moves["Drill Peck"]   = { "Drill Peck",   T::FLYING,   C::PHYSICAL, 80,  100, 20 };
    s_Moves["Wing Attack"]  = { "Wing Attack",  T::FLYING,   C::PHYSICAL, 60,  100, 35 };

    // --- GROUND ---
    s_Moves["Earthquake"]   = { "Earthquake",   T::GROUND,   C::PHYSICAL, 100, 100, 10 };

    // --- DRAGON ---
    s_Moves["Dragon Rage"]  = { "Dragon Rage",  T::DRAGON,   C::SPECIAL,   0,  100, 10 }; // Fixed 40 damage

    // --- BUG ---
    s_Moves["Pin Missile"]  = { "Pin Missile",  T::BUG,      C::PHYSICAL, 25,   95, 20 };
    s_Moves["Twineedle"]    = { "Twineedle",    T::BUG,      C::PHYSICAL, 25,  100, 20 };

    // --- GHOST ---
    s_Moves["Confuse Ray"]  = { "Confuse Ray",  T::GHOST,    C::STATUS,    0,  100, 10 };

    // --- FAIRY --- (Or NORMAL depending on your game's generation constraints)
    s_Moves["Sweet Kiss"]   = { "Sweet Kiss",   T::FAIRY,    C::STATUS,    0,   75, 10 };
    s_Moves["Charm"]        = { "Charm",        T::FAIRY,    C::STATUS,    0,  100, 20 };

    LOG_TRACE("MoveDatabase initialised with {} moves.", s_Moves.size());
}

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
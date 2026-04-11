// PokemonDatabase.cpp
#include "PokemonDatabase.hpp"
#include "MoveDatabase.hpp"
#include "Util/Logger.hpp"
#include <cmath>

std::unordered_map<std::string, PokemonSpecies> PokemonDatabase::s_Species;

int PokemonDatabase::CalcHP(int baseHP, int level) {
    // Simplified Gen 1 HP formula
    return static_cast<int>((baseHP * 2 * level) / 100.0f) + level + 10;
}

int PokemonDatabase::CalcStat(int baseStat, int level) {
    return static_cast<int>((baseStat * 2 * level) / 100.0f) + 5;
}

std::shared_ptr<Pokemon> PokemonDatabase::CreatePokemon(
    const std::string& name, int level) {
    
    if (!HasSpecies(name)) {
        LOG_ERROR("Unknown Pokemon species: {}", name);
        return nullptr;
    }

    const PokemonSpecies& species = GetSpecies(name);

    int hp  = CalcHP(species.baseHP, level);
    int atk = CalcStat(species.baseAttack, level);
    int def = CalcStat(species.baseDefense, level);
    int spa = CalcStat(species.baseSpAttack, level);
    int spd = CalcStat(species.baseSpDefense, level);
    int spe = CalcStat(species.baseSpeed, level);

    auto pokemon = std::make_shared<Pokemon>(
        name, level,
        species.type1, species.type2,
        hp, atk, def, spa, spd, spe);

    // Give all moves learnable at or below this level
    for (const auto& lm : species.levelUpMoves) {
        if (lm.level <= level) {
            pokemon->LearnMove(lm.moveName);
        }
    }

    return pokemon;
}

void PokemonDatabase::Init() {
    // --- GEN 1 STARTERS ---
    s_Species["Charmander"] = {
        4, "Charmander",
        PokemonType::FIRE, PokemonType::NONE,
        39, 52, 43, 60, 50, 65,
        45, 65,
        {
            { 1,  "Scratch" },
            { 1,  "Growl" },
            { 7,  "Ember" },
            { 13, "Smokescreen" },
            { 19, "Rage" },
            { 25, "Slash" },
            { 31, "Flamethrower" },
            { 38, "Fire Spin" }
        },
        "charmander"
    };

    s_Species["Bulbasaur"] = {
        1, "Bulbasaur",
        PokemonType::GRASS, PokemonType::POISON,
        45, 49, 49, 65, 65, 45,
        45, 64,
        {
            { 1,  "Tackle" },
            { 1,  "Growl" },
            { 7,  "Vine Whip" },
            { 13, "Leech Seed" },
            { 22, "Razor Leaf" },
            { 30, "Solar Beam" }
        },
        "bulbasaur"
    };

    s_Species["Squirtle"] = {
        7, "Squirtle",
        PokemonType::WATER, PokemonType::NONE,
        44, 48, 65, 50, 64, 43,
        45, 66,
        {
            { 1,  "Tackle" },
            { 1,  "Tail Whip" },
            { 7,  "Water Gun" },
            { 13, "Withdraw" },
            { 22, "Surf" },
        },
        "squirtle"
    };
    s_Species["Tododile"] = {
        7, "Tododile",
        PokemonType::WATER, PokemonType::NONE,
        44, 48, 65, 50, 64, 43,
        45, 66,
        {
            { 1,  "Tackle" },
            { 1,  "Tail Whip" },
            { 7,  "Water Gun" },
            { 13, "Withdraw" },
            { 22, "Surf" },
        },
        "tododile"
    };

    s_Species["Pikachu"] = {
        25, "Pikachu",
        PokemonType::ELECTRIC, PokemonType::NONE,
        35, 55, 30, 50, 40, 90,
        190, 82,
        {
            { 1,  "Thunder Shock" },
            { 1,  "Growl" },
            { 9,  "Quick Attack" },
            { 16, "Thunderbolt" },
        },
        "pikachu"
    };

    // --- COMMON WILD POKEMON ---
    s_Species["Rattata"] = {
        19, "Rattata",
        PokemonType::NORMAL, PokemonType::NONE,
        30, 56, 35, 25, 35, 72,
        255, 51,
        {
            { 1, "Tackle" },
            { 1, "Tail Whip" },
            { 7, "Quick Attack" },
            { 14, "Hyper Fang" }
        },
        "rattata"
    };

    s_Species["Starly"] = {
        16, "Starly",
        PokemonType::NORMAL, PokemonType::FLYING,
        40, 45, 40, 35, 35, 56,
        255, 55,
        {
            { 1, "Tackle" },
            { 1, "Growl" },
            { 5, "Quick Attack" }
        },
        "starly"
    };

    LOG_TRACE("PokemonDatabase initialised with {} species.", s_Species.size());
}

const PokemonSpecies& PokemonDatabase::GetSpecies(const std::string& name) {
    auto it = s_Species.find(name);
    if (it == s_Species.end()) {
        LOG_ERROR("Species '{}' not found!", name);
        static PokemonSpecies empty{};
        return empty;
    }
    return it->second;
}

bool PokemonDatabase::HasSpecies(const std::string& name) {
    return s_Species.count(name) > 0;
}
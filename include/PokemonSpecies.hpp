// PokemonSpecies.hpp
#pragma once
#include "Pokemon.hpp"
#include <string>
#include <vector>

struct LevelUpMove {
    int level;
    std::string moveName;
};

struct PokemonSpecies {
    // Identity
    int dexNumber;
    std::string name;
    PokemonType type1;
    PokemonType type2;

    // Base stats (before level scaling)
    int baseHP;
    int baseAttack;
    int baseDefense;
    int baseSpAttack;
    int baseSpDefense;
    int baseSpeed;

    // Catch rate (255 = easiest, 3 = legendary)
    int catchRate;

    // Base EXP yield when defeated
    int baseExpYield;

    // Moves learned by level up
    std::vector<LevelUpMove> levelUpMoves;

    // Sprite paths (relative name, actual path built at runtime)
    std::string spriteName; // e.g. "bulbasaur" -> loads "bulbasaur_front_1.png"
};
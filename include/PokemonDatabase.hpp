// PokemonDatabase.hpp
#pragma once
#include "PokemonSpecies.hpp"
#include <unordered_map>
#include <string>
#include <memory>

class PokemonDatabase {
public:
    static void Init();
    static const PokemonSpecies& GetSpecies(const std::string& name);
    static bool HasSpecies(const std::string& name);

    // Factory method — creates a correctly-statted Pokemon instance
    // from species data at a given level
    static std::shared_ptr<Pokemon> CreatePokemon(
        const std::string& name, int level);

private:
    static std::unordered_map<std::string, PokemonSpecies> s_Species;
    
    // Calculates actual stat from base stat and level
    // Simplified formula (not full IV/EV but closer to authentic)
    static int CalcStat(int baseStat, int level);
    static int CalcHP(int baseHP, int level);
};
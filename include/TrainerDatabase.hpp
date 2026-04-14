#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "Pokemon.hpp"

// A blueprint for a single Pokemon in a trainer's team
struct PokemonConfig {
    std::string species;
    int level;
    std::vector<std::string> customMoves;
};

// A blueprint for a full Trainer
struct TrainerData {
    std::string name;
    std::vector<PokemonConfig> party;
};

class TrainerDatabase {
public:
    // Load the JSON and populate s_Trainers
    static void Init();

    // Generate a fresh party of Pokemon pointers for battle
    static std::vector<std::shared_ptr<Pokemon>> CreateTrainerParty(const std::string& trainerID);
    
    // Optional: Get the trainer's display name for dialogue/UI
    static std::string GetTrainerName(const std::string& trainerID);

private:
    static std::unordered_map<std::string, TrainerData> s_Trainers;
};
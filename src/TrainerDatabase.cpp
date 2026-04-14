#include "TrainerDatabase.hpp"
#include "PokemonDatabase.hpp"
#include "Util/Logger.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

std::unordered_map<std::string, TrainerData> TrainerDatabase::s_Trainers;

void TrainerDatabase::Init() {
    // Make sure RESOURCE_DIR is defined, or use your engine's path structure
    std::ifstream file(std::string(RESOURCE_DIR) + "/trainers.json"); 
    
    if (!file.is_open()) {
        LOG_ERROR("TrainerDatabase: Failed to open trainers.json");
        return;
    }

    try {
        json data = json::parse(file);
        
        // Loop through all items in the "trainers" object
        for (auto& [trainerID, trainerJson] : data["trainers"].items()) {
            TrainerData tData;
            tData.name = trainerJson.value("name", "Unknown Trainer");

            // Build the blueprint for each Pokemon
            for (const auto& pokeJson : trainerJson["party"]) {
                PokemonConfig pConfig;
                pConfig.species = pokeJson["species"];
                pConfig.level = pokeJson["level"];

                if (pokeJson.contains("custom_moves")) {
                    for (const auto& move : pokeJson["custom_moves"]) {
                        pConfig.customMoves.push_back(move);
                    }
                }
                tData.party.push_back(pConfig);
            }
            s_Trainers[trainerID] = tData;
        }
        LOG_TRACE("TrainerDatabase initialised with {} trainers.", s_Trainers.size());
    } catch (const std::exception& e) {
        LOG_ERROR("TrainerDatabase: Error parsing JSON: {}", e.what());
    }
}

std::vector<std::shared_ptr<Pokemon>> TrainerDatabase::CreateTrainerParty(const std::string& trainerID) {
    std::vector<std::shared_ptr<Pokemon>> party;
    
    auto it = s_Trainers.find(trainerID);
    if (it == s_Trainers.end()) {
        LOG_ERROR("Trainer ID '{}' not found in database!", trainerID);
        return party;
    }

    const TrainerData& tData = it->second;
    
    // Generate actual Pokemon objects from the blueprints
    for (const auto& pConfig : tData.party) {
        auto pokemon = PokemonDatabase::CreatePokemon(pConfig.species, pConfig.level);
        
        if (pokemon) {
            // Apply custom moves if this config has any
            if (!pConfig.customMoves.empty()) {
                pokemon->ClearMoves(); 
                for (const auto& move : pConfig.customMoves) {
                    pokemon->LearnMove(move);
                }
            }
            party.push_back(pokemon);
        }
    }
    return party;
}

std::string TrainerDatabase::GetTrainerName(const std::string& trainerID) {
    auto it = s_Trainers.find(trainerID);
    if (it != s_Trainers.end()) {
        return it->second.name;
    }
    return "Unknown";
}
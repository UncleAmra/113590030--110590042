#pragma once
#include "GameFlags.hpp"
#include "Item.hpp"
#include "Pokemon.hpp" 
#include "Util/Logger.hpp"
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <nlohmann/json.hpp>


using json = nlohmann::json;

namespace SaveSystem {
    // Let's update the extension to .json!
    const std::string SAVE_PATH = "savegame.json";

    struct GameState {
        std::string mapPath;
        int gridX;
        int gridY;
        int direction; 
        std::unordered_map<std::string, InventoryData> inventory;
        std::unordered_set<std::string> lootedItems;
        std::vector<std::shared_ptr<Pokemon>> party; 
    };

    inline void SaveGame(const GameState& state) {
        json j;

        // 1. Core Player Data
        j["mapPath"] = state.mapPath;
        j["gridX"] = state.gridX;
        j["gridY"] = state.gridY;
        j["direction"] = state.direction;

        // 2. Flags
        j["flags"] = json::object();
        for (const auto& [name, value] : GameFlags::Flags) {
            j["flags"][name] = value;
        }

        // 3. Looted Items (nlohmann can serialize unordered_set directly!)
        j["lootedItems"] = state.lootedItems;

        // 4. Inventory
        j["inventory"] = json::object();
        for (const auto& [itemName, data] : state.inventory) {
            j["inventory"][itemName] = data.quantity;
        }

        // 5. Pokemon Party
        j["party"] = json::array();
        for (const auto& p : state.party) {
            json pkmnJson;
            pkmnJson["name"] = p->GetName();
            pkmnJson["level"] = p->GetLevel();
            pkmnJson["type1"] = static_cast<int>(p->GetType1());
            pkmnJson["type2"] = static_cast<int>(p->GetType2());
            pkmnJson["maxHp"] = p->GetMaxHP();
            pkmnJson["currentHp"] = p->GetCurrentHP();
            pkmnJson["atk"] = p->GetAttack();
            pkmnJson["def"] = p->GetDefense();
            pkmnJson["spa"] = p->GetSpecialAttack();
            pkmnJson["spd"] = p->GetSpecialDefense();
            pkmnJson["spe"] = p->GetSpeed();
            pkmnJson["exp"] = p->GetCurrentExp();
            pkmnJson["catchRate"] = p->GetCatchRate(); // Good idea to save this!
            
            // Moves vector can be dumped straight in
            pkmnJson["moves"] = p->GetMoves(); 

            j["party"].push_back(pkmnJson);
        }

        // Write to file with pretty-printing (4 spaces indent)
        std::ofstream outFile(SAVE_PATH);
        if (outFile.is_open()) {
            outFile << j.dump(4);
            outFile.close();
            LOG_INFO("Game Saved as JSON: Map={}, Pos={},{}", state.mapPath, state.gridX, state.gridY);
        }
    }

    inline bool LoadGame(GameState& outState) {
        if (!std::filesystem::exists(SAVE_PATH)) return false;

        std::ifstream inFile(SAVE_PATH);
        if (!inFile.is_open()) return false;

        json j;
        inFile >> j; // Parse the entire JSON file in one line!

        // 1. Core Player Data (Using .value() provides safe defaults if data is missing)
        outState.mapPath = j.value("mapPath", "");
        outState.gridX = j.value("gridX", 0);
        outState.gridY = j.value("gridY", 0);
        outState.direction = j.value("direction", 0);

        // 2. Flags
        if (j.contains("flags")) {
            for (auto& [key, value] : j["flags"].items()) {
                GameFlags::Set(key, value.get<bool>());
            }
        }

        // 3. Looted Items
        if (j.contains("lootedItems")) {
            outState.lootedItems = j["lootedItems"].get<std::unordered_set<std::string>>();
        }

        // 4. Inventory
        if (j.contains("inventory")) {
            for (auto& [key, value] : j["inventory"].items()) {
                outState.inventory[key].quantity = value.get<int>();
            }
        }

        // 5. Pokemon Party
        if (j.contains("party")) {
            for (const auto& pkmnJson : j["party"]) {
                std::string name = pkmnJson.value("name", "Unknown");
                int lvl = pkmnJson.value("level", 1);
                auto t1 = static_cast<PokemonType>(pkmnJson.value("type1", 0));
                auto t2 = static_cast<PokemonType>(pkmnJson.value("type2", 0));
                int mhp = pkmnJson.value("maxHp", 10);
                int atk = pkmnJson.value("atk", 5);
                int def = pkmnJson.value("def", 5);
                int spa = pkmnJson.value("spa", 5);
                int spd = pkmnJson.value("spd", 5);
                int spe = pkmnJson.value("spe", 5);
                
                // Added the 11th argument here to satisfy your constructor!
                int catchRate = pkmnJson.value("catchRate", 45); 

                auto pkmn = std::make_shared<Pokemon>(
                    name, lvl, t1, t2, mhp, atk, def, spa, spd, spe, catchRate
                );
                
                // Set the volatile stats
                pkmn->SetCurrentHP(pkmnJson.value("currentHp", mhp));
                pkmn->SetCurrentExp(pkmnJson.value("exp", 0));

                // Read all moves
                if (pkmnJson.contains("moves")) {
                    for (const auto& move : pkmnJson["moves"]) {
                        pkmn->LearnMove(move.get<std::string>());
                    }
                }
                
                outState.party.push_back(pkmn);
            }
        }

        return true;
    }
}
#pragma once
#include "GameFlags.hpp"
#include "Item.hpp"
#include "Pokemon.hpp" // <-- Must include this now!
#include "Util/Logger.hpp"
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <memory>

namespace SaveSystem {
    const std::string SAVE_PATH = "savegame.txt";

    struct GameState {
        std::string mapPath;
        int gridX;
        int gridY;
        int direction; 
        std::unordered_map<std::string, InventoryData> inventory;
        std::unordered_set<std::string> lootedItems;
        
        // ---> NEW: Store the party! <---
        std::vector<std::shared_ptr<Pokemon>> party; 
    };

    inline void SaveGame(const GameState& state) {
        std::ofstream outFile(SAVE_PATH);
        if (!outFile.is_open()) return;

        outFile << "MAP " << state.mapPath << "\n";
        outFile << "POS " << state.gridX << " " << state.gridY << "\n";
        outFile << "DIR " << state.direction << "\n";

        for (const auto& [name, value] : GameFlags::Flags) {
            outFile << "FLAG " << name << " " << (value ? "1" : "0") << "\n";
        }

        for (const auto& itemID : state.lootedItems) {
            outFile << "LOOTED " << itemID << "\n";
        }

        for (const auto& [itemName, qty] : state.inventory) {
            outFile << "INV " << itemName << " " << qty.quantity << "\n";
        }

        // ---> NEW: Save the Pokemon Party <---
        for (const auto& p : state.party) {
            // Save core stats
            outFile << "PKMN " << p->GetName() << " " << p->GetLevel() << " " 
                    << static_cast<int>(p->GetType1()) << " " << static_cast<int>(p->GetType2()) << " "
                    << p->GetMaxHP() << " " << p->GetCurrentHP() << " "
                    << p->GetAttack() << " " << p->GetDefense() << " "
                    << p->GetSpecialAttack() << " " << p->GetSpecialDefense() << " "
                    << p->GetSpeed() << " " << p->GetCurrentExp();
            
            // Save moves
            auto moves = p->GetMoves();
            outFile << " " << moves.size();
            for (const auto& move : moves) {
                outFile << " " << move;
            }
            outFile << "\n";
        }

        outFile.close();
        LOG_INFO("Game Saved: Map={}, Pos={},{}", state.mapPath, state.gridX, state.gridY);
    }

    inline bool LoadGame(GameState& outState) {
        if (!std::filesystem::exists(SAVE_PATH)) return false;

        std::ifstream inFile(SAVE_PATH);
        std::string type;
        
        while (inFile >> type) {
            if (type == "MAP")  inFile >> outState.mapPath;
            else if (type == "POS")  inFile >> outState.gridX >> outState.gridY;
            else if (type == "DIR")  inFile >> outState.direction;
            else if (type == "FLAG") {
                std::string name;
                bool val;
                inFile >> name >> val;
                GameFlags::Set(name, val);
            }
            else if (type == "LOOTED") {
                std::string itemID;
                inFile >> itemID;
                outState.lootedItems.insert(itemID);
            }
            else if (type == "INV") {
                std::string itemName;
                int qty;
                inFile >> itemName >> qty;
                outState.inventory[itemName].quantity = qty;
            }
            // ---> NEW: Read the Pokemon Party <---
            else if (type == "PKMN") {
                std::string name;
                int lvl, t1, t2, mhp, chp, atk, def, spa, spd, spe, exp, moveCount;
                
                inFile >> name >> lvl >> t1 >> t2 >> mhp >> chp >> atk >> def >> spa >> spd >> spe >> exp >> moveCount;
                
                auto pkmn = std::make_shared<Pokemon>(
                    name, lvl, static_cast<PokemonType>(t1), static_cast<PokemonType>(t2),
                    mhp, atk, def, spa, spd, spe
                );
                
                // Override the default HP and EXP so they don't accidentally level up or heal
                pkmn->SetCurrentHP(chp);
                pkmn->SetCurrentExp(exp);

                // Read all moves
                for(int i = 0; i < moveCount; i++) {
                    std::string moveName;
                    inFile >> moveName;
                    pkmn->LearnMove(moveName);
                }
                
                outState.party.push_back(pkmn);
            }
        }
        return true;
    }
}
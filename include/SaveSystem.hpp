#pragma once
#include "GameFlags.hpp"
#include "Item.hpp"
#include "Util/Logger.hpp"
#include <fstream>
#include <string>
#include <filesystem>

namespace SaveSystem {
    const std::string SAVE_PATH = "savegame.txt";

    struct GameState {
        std::string mapPath;
        int gridX;
        int gridY;
        int direction; // Store as int (0-3)
        std::unordered_map<std::string, int> inventory;
        std::unordered_set<std::string> lootedItems;
    };

    // 1. SAVE: Write Map, Position, and then all Flags
    inline void SaveGame(const GameState& state) {
        std::ofstream outFile(SAVE_PATH);
        if (!outFile.is_open()) return;

        // Save Header Info
        outFile << "MAP " << state.mapPath << "\n";
        outFile << "POS " << state.gridX << " " << state.gridY << "\n";
        outFile << "DIR " << state.direction << "\n";

        // Save All Story Flags
        for (const auto& [name, value] : GameFlags::Flags) {
            outFile << "FLAG " << name << " " << (value ? "1" : "0") << "\n";
        }

        // ---> NEW: Save Looted Items <---
        for (const auto& itemID : state.lootedItems) {
            outFile << "LOOTED " << itemID << "\n";
        }

        // ---> NEW: Save Inventory <---
        for (const auto& [itemName, qty] : state.inventory) {
            outFile << "INV " << itemName << " " << qty << "\n";
        }

        outFile.close();
        LOG_INFO("Game Saved: Map={}, Pos={},{}", state.mapPath, state.gridX, state.gridY);
    }

    // 2. LOAD: Read the file line by line
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
            // ---> NEW: Read Looted Items <---
            else if (type == "LOOTED") {
                std::string itemID;
                inFile >> itemID;
                outState.lootedItems.insert(itemID);
            }
            // ---> NEW: Read Inventory <---
            else if (type == "INV") {
                std::string itemName;
                int qty;
                inFile >> itemName >> qty;
                outState.inventory[itemName] = qty;
            }
        }
        return true;
    }
}
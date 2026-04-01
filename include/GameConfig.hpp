#ifndef GAMECONFIG_HPP
#define GAMECONFIG_HPP
#include "config.hpp"
#include <string> 
#include <unordered_set> 
#include <unordered_map>

//Check Map.cpp to find the IDs for items/tiles 

namespace GameConfig {
    // --- TILE & GRID SETTINGS ---
    constexpr float TILE_SIZE = 16.0f;
    constexpr float SCALE = 3.0f;
    constexpr float SCALED_TILE_SIZE = TILE_SIZE * SCALE; // 48.0f
    
    // THE FIX: Use this for all position/warp math to prevent drift!
    constexpr float EFFECTIVE_TILE_SIZE = SCALED_TILE_SIZE - 0.1f; 

    // --- CAMERA SETTINGS ---
    constexpr float CAMERA_START_X = -288.0f; 
    constexpr float CAMERA_START_Y = 288.0f;

    // --- TILE IDs ---
    constexpr int TILE_DOOR = 6;
    constexpr int TILE_EXIT = 99; // (Make sure 99 matches your actual exit ID!)

    // --- PROP IDs ---
    constexpr int PROP_POKECENTER = 3;
    constexpr int PROP_CHURCH = 5;
    constexpr int PROP_DOORMAT = 7;
    constexpr int PROP_INVISIBLE_EXIT = 8;
    constexpr int PROP_INVISIBLE_WALL = 999;
    constexpr int PROP_PC_DESK = 10;
    constexpr int PROP_PC_WALL_LEFT = 12;
    constexpr int PROP_PC_WALL_RIGHT = 13;
    constexpr int PROP_CHECKPOINT = 15;
    constexpr int PROP_CHECKPOINT2 = 16;

    //NPC IDs
    constexpr int NPC_NURSE = 100;
    constexpr int NPC_TA1 = 101;

    struct WarpDestination {
        std::string levelPath;
        int spawnX;
        int spawnY;
    };

    //registry for items that have already been looted so they don't respawn
    inline std::unordered_set<std::string> LootedItems;
// Define our destinations (Where the player SPAWNS after teleporting)
    inline const WarpDestination WARP_PC_INSIDE = { RESOURCE_DIR "/maps/inside", 7, 8 };
    inline const WarpDestination WARP_TOWN_OUTSIDE = { RESOURCE_DIR "/maps/level", 15, 7 };
    inline const WarpDestination WARP_NTUT = { RESOURCE_DIR "/maps/NTUT",9,3 };
    inline const WarpDestination WARP_TOWN_FROM_NTUT = { RESOURCE_DIR "/maps/level", 4, 3 };
    // A dictionary that maps: "CurrentMap_DoorX_DoorY" -> "Where the door goes"
    inline std::unordered_map<std::string, WarpDestination> DoorRouting = {
        
        { RESOURCE_DIR "/maps/level_15_7", WARP_PC_INSIDE },
        { RESOURCE_DIR "/maps/inside_7_8", WARP_TOWN_OUTSIDE },
        { RESOURCE_DIR "/maps/level_4_3", WARP_NTUT },
        { RESOURCE_DIR "/maps/NTUT_9_3", WARP_TOWN_FROM_NTUT }
        
    };
}
#endif
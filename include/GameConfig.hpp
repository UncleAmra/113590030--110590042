#ifndef GAMECONFIG_HPP
#define GAMECONFIG_HPP
#include "config.hpp"
#include <string> // Added this so std::string works!

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
    constexpr int NPC_NURSE = 100;

    struct WarpDestination {
        std::string levelPath;
        int spawnX;
        int spawnY;
    };

    // Define our current doors!
    inline const WarpDestination WARP_PC_INSIDE = { RESOURCE_DIR "/inside", 7, 9 };
    inline const WarpDestination WARP_TOWN_OUTSIDE = { RESOURCE_DIR "/level", 15, 7 };
}
#endif
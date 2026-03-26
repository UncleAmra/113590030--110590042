#ifndef MAP_HPP
#define MAP_HPP

#include "pch.hpp"
#include "NPC.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "Util/Animation.hpp"
#include <vector>
#include <string>
#include <memory>
#include <unordered_map> 

// ADDED: The blueprint for every tile!
struct TileProperties {
    std::shared_ptr<Util::Image> texture;
    float zIndex = 0.0f;
    float yOffset = 0.0f;
    bool isWalkable = false; // We can handle walkability here now!
};
struct PropProperties {
    std::string texturePath;
    float zIndex;
    bool dynamicZ;
};
class Prop;

class Map : public Util::GameObject {
public:
    Map(); 
    void WarpTo(int gridX, int gridY);
    int GetPropType(int gridX, int gridY);
    void Move(float dx, float dy);
    void Draw(); 
    bool IsWalkable(int x, int y);
    
    void Update();
    int GetTileType(int gridX, int gridY);
    void LoadLevel(const std::string& filepath);

private:
    // --- MAP DATA ---
    std::vector<std::shared_ptr<NPC>> m_NPCs;
    std::vector<std::vector<int>> m_PropData;
    std::vector<std::vector<int>> m_LevelData;
    std::vector<std::shared_ptr<Util::GameObject>> m_Tiles;
    std::vector<std::shared_ptr<Prop>> m_Props;
    
    // --- THE REGISTRY ---
    std::unordered_map<int, TileProperties> m_TileRegistry;
    std::unordered_map<int, std::string> m_NPCRegistry;   
    std::unordered_map<int, PropProperties> m_PropRegistry; 
    void InitTileRegistry(); 
    void InitPropRegistry();
    void InitNPCRegistry(); // A helper to set up the dictionary

    // --- ANIMATIONS ---
    std::shared_ptr<Util::Animation> m_LeaderWater;
    std::shared_ptr<Util::Animation> m_FollowerWater;

    // (Consider deleting these if Util::Animation handles your water now!)
    float m_WaterTimer = 0.0f;
    bool m_IsWaterFrameOne = true;
    int m_CurrentWaterFrame = 0;
    std::vector<std::shared_ptr<Util::GameObject>> m_WaterTiles;
    std::vector<std::shared_ptr<Util::Image>> m_WaterFrames;

    // --- TILE IMAGES ---
    // Pro-tip: If you only use these inside InitTileRegistry() to load 
    // the dictionary, you don't actually need to save them as member variables!
    std::shared_ptr<Util::Image> m_GrassImage;
    std::shared_ptr<Util::Image> m_PCfloorTile;
    std::shared_ptr<Util::Image> m_WaterImage;
    std::shared_ptr<Util::Image> m_WaterImage2;
    std::shared_ptr<Util::Image> m_WaterImage3;
    std::shared_ptr<Util::Image> m_DirtImage;
    std::shared_ptr<Util::Image> m_PokeCentreImage;
    std::shared_ptr<Util::Image> m_ChurchImage;  
    std::shared_ptr<Util::Image> m_PCDoorImage;
    std::shared_ptr<Util::Image> m_PCDesk;
    std::shared_ptr<Util::Image> m_PCWall1;
    std::shared_ptr<Util::Image> m_PCWall2;
    std::shared_ptr<Util::Image> m_PCWall3;
    std::shared_ptr<Util::Image> m_PokeCentreInsideImage;

    // --- HELPER FUNCTIONS ---
    std::vector<std::vector<int>> LoadCSV(const std::string& filepath);
    void ClearMap();
};

#endif
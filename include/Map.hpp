#ifndef MAP_HPP
#define MAP_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <vector>
#include <memory>
#include <unordered_map> // ADDED FOR OUR DICTIONARY

// ADDED: The blueprint for every tile!
struct TileProperties {
    std::shared_ptr<Util::Image> texture;
    float zIndex = 0.0f;
    float yOffset = 0.0f;
    bool isWalkable = false; // We can handle walkability here now!
};


class Prop;

class Map : public Util::GameObject {
public:
    Map(); 
    
    void Move(float dx, float dy);
    void Draw(); 
    bool IsWalkable(int x, int y);
    
    float m_WaterTimer = 0.0f;
    bool m_IsWaterFrameOne = true;
    int m_CurrentWaterFrame = 0;
    
    void Update(float deltaTime);
    int GetTileType(int gridX, int gridY);
    void LoadLevel(const std::string& filepath);

private:
    std::vector<std::vector<int>> m_PropData;
    std::vector<std::shared_ptr<Util::GameObject>> m_Tiles;
    std::vector<std::shared_ptr<Prop>> m_Props;
    std::vector<std::vector<int>> m_LevelData;
    
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
    std::vector<std::shared_ptr<Util::Image>> m_WaterFrames;

    // ADDED: The Dictionary that holds all our tile rules
    std::unordered_map<int, TileProperties> m_TileRegistry;

    void InitTileRegistry(); // ADDED: Helper function to set up rules
    std::vector<std::vector<int>> LoadCSV(const std::string& filepath);
    //void LoadMapFromFile(const std::string& filepath);
    void ClearMap();

    std::vector<std::shared_ptr<Util::GameObject>> m_WaterTiles;
};

#endif
#ifndef MAP_HPP
#define MAP_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <vector>
#include <memory>

class Map : public Util::GameObject {
public:
    Map(); 
    
    void Move(float dx, float dy);
    void Draw(); 
    bool IsWalkable(int x, int y);
    // Animation variables
    float m_WaterTimer = 0.0f;
    bool m_IsWaterFrameOne = true;
    int m_CurrentWaterFrame = 0;
    void Update(float deltaTime);
    int GetTileType(int gridX, int gridY);
    void LoadLevel(const std::string& filepath);

private:
    std::vector<std::shared_ptr<Util::GameObject>> m_Tiles;
    std::vector<std::vector<int>> m_LevelData;
    std::shared_ptr<Util::Image> m_GrassImage;
    std::shared_ptr<Util::Image> m_WaterImage;
    std::shared_ptr<Util::Image> m_WaterImage2;
    std::shared_ptr<Util::Image> m_WaterImage3;
    std::shared_ptr<Util::Image> m_DirtImage;
    std::shared_ptr<Util::Image> m_PokeCentreImage;
    std::shared_ptr<Util::Image> m_ChurchImage;  
    std::shared_ptr<Util::Image>m_PCDoorImage;
    std::vector<std::shared_ptr<Util::Image>> m_WaterFrames;
   

    void LoadMapFromFile(const std::string& filepath);

    // A list to hold ONLY the water tiles for fast updating
    std::vector<std::shared_ptr<Util::GameObject>> m_WaterTiles;
    //void LoadLevel(const std::string& filepath);
    void ClearMap(); // To delete the old tiles
};

#endif
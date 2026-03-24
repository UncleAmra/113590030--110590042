#include "Map.hpp"
#include <fstream>  
#include <sstream>  
#include <iostream> 

Map::Map() {
    // 1. LOAD IMAGES ONLY ONCE
    m_GrassImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Grass1.png");
    m_WaterFrames.push_back(std::make_shared<Util::Image>(RESOURCE_DIR "/Water1.png"));
    m_WaterFrames.push_back(std::make_shared<Util::Image>(RESOURCE_DIR "/Water2.png"));
    m_WaterFrames.push_back(std::make_shared<Util::Image>(RESOURCE_DIR "/Water3.png"));
    

    m_DirtImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Dirt1.png");
    m_PokeCentreImage = std::make_shared<Util::Image>(RESOURCE_DIR "/PokeCentre.png");
    m_PokeCentreInsideImage = std::make_shared<Util::Image>(RESOURCE_DIR "/PokeCentreInside.png");
    
    m_PCDesk = std::make_shared<Util::Image>(RESOURCE_DIR "/PCDesk.png");
    m_PCWall1 = std::make_shared<Util::Image>(RESOURCE_DIR "/PCWall1.png");
    m_PCWall2 = std::make_shared<Util::Image>(RESOURCE_DIR "/PCWall2.png");
    m_PCWall3 = std::make_shared<Util::Image>(RESOURCE_DIR "/PCWall3.png");
    m_PCDoorImage = std::make_shared<Util::Image>(RESOURCE_DIR "/PC_doormat.png");
    m_PCfloorTile = std::make_shared<Util::Image>(RESOURCE_DIR "/PCFloorTile.png");
    m_ChurchImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Church.png");

    // 2. SETUP THE DICTIONARY
    InitTileRegistry();

    // 3. BUILD THE FIRST MAP
    LoadLevel(RESOURCE_DIR "/level.csv");
}

void Map::InitTileRegistry() {
    // ID = { texture, zIndex, yOffset, isWalkable }
    m_TileRegistry[0] = { m_GrassImage, 0.0f, 0.0f, true };
    m_TileRegistry[1] = { m_WaterFrames[0], 0.0f, 0.0f, false };
    m_TileRegistry[2] = { m_DirtImage, 0.0f, 0.0f, true };
    m_TileRegistry[3] = { m_PokeCentreImage, 0.1f, 24.0f, false };
    m_TileRegistry[4] = { m_GrassImage, 0.0f, 0.0f, false }; 
    m_TileRegistry[41] = { m_DirtImage, 0.0f, 0.0f, false }; 
    m_TileRegistry[5] = { m_ChurchImage, 0.5f, 0.0f, false };
    m_TileRegistry[6] = { m_DirtImage, 0.0f, 0.0f, false }; // Door (acts as wall)
    m_TileRegistry[7] = { m_PCDoorImage, 0.5f, 0.0f, false }; // Inside mat (acts as wall)
    m_TileRegistry[8] = { m_PokeCentreInsideImage, 0.3f, 0.0f, true }; // Inside floor
    m_TileRegistry[9] = { m_PCfloorTile, 0.0f, 0.0f, true }; //  PokeCentre floor tile
    m_TileRegistry[10] = { m_PCDesk, 0.3f, 0.0f, true };  
    m_TileRegistry[11] = { m_PCWall1, 0.0f, 0.0f, false }; 
    m_TileRegistry[12] = { m_PCWall2, 0.1f, 0.0f, false }; //  PokeCentre floor tile
    m_TileRegistry[13] = { m_PCWall3, 0.1f, 0.0f, false }; //  PokeCentre floor tile




}

void Map::LoadLevel(const std::string& filepath) {
    ClearMap(); 
    LoadMapFromFile(filepath);

    if (m_LevelData.empty()) {
        std::cout << "Map is EMPTY! Check your file path: " << filepath << std::endl;
        return;
    }

    float tileSize = 16.0f; 
    float scale = 3.0f;     
    float scaledTileSize = (tileSize * scale)-0.1f; 
    float startX = -288.0f; 
    float startY = 288.0f;

    // THE BEAUTIFUL NEW DOUBLE LOOP
    for (size_t y = 0; y < m_LevelData.size(); y++) {
        for (size_t x = 0; x < m_LevelData[y].size(); x++) {
            
            int tileID = m_LevelData[y][x];

            // If the ID exists in our dictionary, build it!
            if (m_TileRegistry.count(tileID) > 0) {
                auto newTile = std::make_shared<Util::GameObject>();
                const TileProperties& props = m_TileRegistry[tileID];

                newTile->m_Transform.scale = {scale, scale};
                newTile->m_Transform.translation.x = startX + (x * scaledTileSize);
                newTile->m_Transform.translation.y = startY - (y * scaledTileSize) + props.yOffset;

                newTile->SetDrawable(props.texture);
                newTile->SetZIndex(props.zIndex);
                
                // Track water for animation
                if (tileID == 1) { m_WaterTiles.push_back(newTile); }

                m_Tiles.push_back(newTile);
            }
        }
    }
}

void Map::Move(float dx, float dy) {
    for (auto& tile : m_Tiles) {
        tile->m_Transform.translation.x += dx;
        tile->m_Transform.translation.y += dy;
    }
}

void Map::Draw() {
    for (auto& tile : m_Tiles) {
        tile->Draw();
    }
}

bool Map::IsWalkable(int x, int y) {
    // Bounds check
    if (y < 0 || y >= static_cast<int>(m_LevelData.size()) || 
        x < 0 || x >= static_cast<int>(m_LevelData[0].size())) {
        return false; 
    }
    
    int tileType = m_LevelData[y][x];
    
    // Look up the boolean directly from our dictionary!
    if (m_TileRegistry.count(tileType) > 0) {
        return m_TileRegistry[tileType].isWalkable;
    }
    
    return false; 
}

void Map::LoadMapFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open map file at " << filepath << "!\n";
        return;
    }
    m_LevelData.clear(); 
    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')) {
            row.push_back(std::stoi(cell)); 
        }
        m_LevelData.push_back(row);
    }
    file.close();
}

void Map::Update(float deltaTime) {
    m_WaterTimer += deltaTime;
    if (m_WaterTimer >= 0.5f) {
        m_WaterTimer = 0.0f; 
        m_CurrentWaterFrame = (m_CurrentWaterFrame + 1) % m_WaterFrames.size();
        for (auto& waterTile : m_WaterTiles) {
            waterTile->SetDrawable(m_WaterFrames[m_CurrentWaterFrame]);
        }
    }
}

int Map::GetTileType(int gridX, int gridY) {
    if (gridY < 0 || gridY >= static_cast<int>(m_LevelData.size()) || 
        gridX < 0 || gridX >= static_cast<int>(m_LevelData[0].size())) {
        return -1; 
    }
    return m_LevelData[gridY][gridX];
}

void Map::ClearMap() {
    m_Tiles.clear();
    m_WaterTiles.clear();
    m_LevelData.clear();
}
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
    m_PCDoorImage = std::make_shared<Util::Image>(RESOURCE_DIR "/PC_doormat.png");
    m_ChurchImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Church.png");

    // 2. BUILD THE FIRST MAP
    LoadLevel(RESOURCE_DIR "/level.csv");
}

void Map::LoadLevel(const std::string& filepath) {
    // 1. Erase whatever map was here before
    ClearMap(); 
    
    // 2. Read the new CSV
    LoadMapFromFile(filepath);

    if (m_LevelData.empty()) {
        std::cout << "Map is EMPTY! Check your file path: " << filepath << std::endl;
        return;
    } else {
        std::cout << "Map Loaded! Rows: " << m_LevelData.size() << " Columns: " << m_LevelData[0].size() << std::endl;
    }

    // 3. Build the tiles! (Moved here from the constructor)
    float tileSize = 16.0f; 
    float scale = 3.0f;     
    float scaledTileSize = tileSize * scale; 
    float startX = -288.0f; 
    float startY = 288.0f;

    for (size_t y = 0; y < m_LevelData.size(); y++) {
        for (size_t x = 0; x < m_LevelData[y].size(); x++) {
            auto newTile = std::make_shared<Util::GameObject>();
            float zIndex = 0.0f;

            newTile->m_Transform.scale = {scale, scale};
            newTile->m_Transform.translation.x = startX + (x * scaledTileSize);
            newTile->m_Transform.translation.y = startY - (y * scaledTileSize);

            if (m_LevelData[y][x] == 0) {
                newTile->SetDrawable(m_GrassImage);
            } else if (m_LevelData[y][x] == 1) {
                newTile->SetDrawable(m_WaterFrames[0]);
                m_WaterTiles.push_back(newTile);
            } else if (m_LevelData[y][x] == 2) {
                newTile->SetDrawable(m_DirtImage);
            } else if (m_LevelData[y][x] == 3) {
                newTile->SetDrawable(m_PokeCentreImage); 
                zIndex = 0.3f; 
                newTile->m_Transform.translation.y += 24.0f;
            } else if (m_LevelData[y][x] == 4) {
                newTile->SetDrawable(m_GrassImage); 
            } else if (m_LevelData[y][x] == 5) {
                newTile->SetDrawable(m_ChurchImage);
                zIndex = 0.5f; 
            } else if (m_LevelData[y][x] == 6) {
                newTile->SetDrawable(m_DirtImage); // THE DOOR!
            } else if (m_LevelData[y][x] == 7) {
                
                newTile->SetDrawable(m_PCDoorImage); // INSIDE EXIT MAT
                zIndex = 0.5f; 
            }

            newTile->SetZIndex(zIndex); 
            m_Tiles.push_back(newTile);
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
    if (y < 0 || y >= static_cast<int>(m_LevelData.size()) || 
        x < 0 || x >= static_cast<int>(m_LevelData[0].size())) {
        return false; 
    }
    
    int tileType = m_LevelData[y][x];
    
    // 0 = Grass, 2 = Dirt. 
    // Notice 6 is NOT here! It acts like a wall so Red bumps it.
    if (tileType == 0 || tileType == 2) {
        return true;
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
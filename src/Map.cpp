#include "Map.hpp"
#include <fstream>  // For reading files
#include <sstream>  // For breaking strings apart at the commas
#include <iostream> // For printing error messages

Map::Map() {
    m_GrassImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Grass1.png");
    m_WaterImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Water1.png");
    m_DirtImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Dirt1.png");
    m_PokeCentreImage = std::make_shared<Util::Image>(RESOURCE_DIR "/PokeCentre.png");
    m_ChurchImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Church.png");

    LoadMapFromFile(RESOURCE_DIR "/level.csv");
 

// DEBUG CHECK:
if (m_LevelData.empty()) {
    std::cout << "Map is EMPTY! Check your file path." << std::endl;
} else {
    std::cout << "Map Loaded! Rows: " << m_LevelData.size() << " Columns: " << m_LevelData[0].size() << std::endl;
}

    /*
    m_LevelData = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 0, 2, 0, 1, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1},
        {1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1},
        {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    */


    float tileSize = 16.0f; 
    float scale = 3.0f;     
    float scaledTileSize = tileSize * scale; 

    float startX = -288.0f; 
    float startY = 288.0f;

for (size_t y = 0; y < m_LevelData.size(); y++) {
    for (size_t x = 0; x < m_LevelData[y].size(); x++) {
        auto newTile = std::make_shared<Util::GameObject>();

        // Default Z-Index for everything
        float zIndex = 0.0f;

        // Base tile transform and scale
        newTile->m_Transform.scale = {scale, scale};
        newTile->m_Transform.translation.x = startX + (x * scaledTileSize);
        newTile->m_Transform.translation.y = startY - (y * scaledTileSize);

        if (m_LevelData[y][x] == 0) {
            newTile->SetDrawable(m_GrassImage);
        } else if (m_LevelData[y][x] == 1) {
            newTile->SetDrawable(m_WaterImage);
        } else if (m_LevelData[y][x] == 2) {
            newTile->SetDrawable(m_DirtImage);
        } else if (m_LevelData[y][x] == 3) {
            newTile->SetDrawable(m_PokeCentreImage);
            zIndex = 0.5f; // Keep it on top of the grass
            newTile->m_Transform.translation.x += (scaledTileSize * 2.0f);
            newTile->m_Transform.translation.y += (scaledTileSize * 2.0f) + 24.0f;
        } else if (m_LevelData[y][x] == 4) {
            newTile->SetDrawable(m_GrassImage);
        } else if (m_LevelData[y][x] == 5) {
            newTile->SetDrawable(m_ChurchImage);
            zIndex = 0.5f;
            newTile->m_Transform.translation.x += (scaledTileSize * 2.0f);
            newTile->m_Transform.translation.y += (scaledTileSize * 2.0f);
        }

        newTile->SetZIndex(zIndex); // Set it ONCE here

        m_Tiles.push_back(newTile);
    }
}
}

void Map::Move(float dx, float dy) {
    for (auto& tile : m_Tiles) {
        // Access the built-in transform
        tile->m_Transform.translation.x += dx;
        tile->m_Transform.translation.y += dy;
    }
}

void Map::Draw() {
    for (auto& tile : m_Tiles) {
        // Now we can just use the GameObject's built-in Draw method!
        tile->Draw();
    }
}
bool Map::IsWalkable(int x, int y) {
    // 1. BOUNDARY CHECK: Use .size() so it automatically adapts to your CSV size
    if (y < 0 || y >= static_cast<int>(m_LevelData.size()) || 
        x < 0 || x >= static_cast<int>(m_LevelData[0].size())) {
        return false; 
    }
    
    // 2. GET TILE TYPE
    int tileType = m_LevelData[y][x];
    
    // 3. DEFINE WALKABLE TILES
    // 0 = Grass, 2 = Dirt. 
    // 1 (Water), 3 (PokeCentre Anchor), and 4 (PokeCentre Walls) should BLOCK.
    if (tileType == 0 || tileType == 2) {
        return true;
    }

    return false; // Everything else is a wall
}

void Map::LoadMapFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    
    // Safety check: Did the file actually open?
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open map file at " << filepath << "!\n";
        return;
    }

    m_LevelData.clear(); // Empty out whatever was in there before
    std::string line;

    // Read the file line by line
    while (std::getline(file, line)) {
        std::vector<int> row;
        std::stringstream ss(line);
        std::string cell;

        // Break the line apart wherever there is a comma
        while (std::getline(ss, cell, ',')) {
            row.push_back(std::stoi(cell)); // Convert the text "0" into the integer 0
        }
        
        m_LevelData.push_back(row);
    }
    
    file.close();
}
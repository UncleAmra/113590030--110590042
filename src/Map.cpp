#include "Map.hpp"
#include "Prop.hpp"
#include "GameConfig.hpp" // <-- 1. Added our new config file!
#include <fstream>  
#include <sstream>  
#include <iostream> 

Map::Map() {
    // 1. LOAD IMAGES ONLY ONCE
    m_GrassImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Grass1.png");
    
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
    LoadLevel(RESOURCE_DIR "/level");
}

void Map::InitTileRegistry() {
    // ID = { texture, zIndex, yOffset, isWalkable }
    m_TileRegistry[0] = { m_GrassImage, 0.0f, 0.0f, true };
    m_TileRegistry[1] = { nullptr, 0.0f, 0.0f, false };
    m_TileRegistry[2] = { m_DirtImage, 0.0f, 0.0f, true };
    m_TileRegistry[3] = { m_PokeCentreImage, 0.1f, 24.0f, false };
    m_TileRegistry[4] = { m_GrassImage, -0.1f, 0.0f, false }; 
    m_TileRegistry[41] = { m_DirtImage, 0.0f, 0.0f, false }; 
    m_TileRegistry[5] = { m_ChurchImage, 0.5f, 0.0f, false };
    m_TileRegistry[6] = { m_DirtImage, 0.0f, 0.0f, false }; // Door (acts as wall)
    m_TileRegistry[7] = { m_PCDoorImage, 0.1f, 0.0f, false }; // Inside mat (acts as wall)
    m_TileRegistry[8] = { m_PokeCentreInsideImage, 0.3f, 0.0f, true }; // Inside floor
    m_TileRegistry[9] = { m_PCfloorTile, 0.0f, 0.0f, true }; //  PokeCentre floor tile
    m_TileRegistry[10] = { m_PCDesk, 0.3f, 0.0f, true };  
    m_TileRegistry[11] = { m_PCWall1, 0.0f, 0.0f, false }; 
    m_TileRegistry[12] = { m_PCWall2, 0.0f, 0.0f, false }; //  PokeCentre floor tile
    m_TileRegistry[13] = { m_PCWall3, 0.0f, 0.0f, false }; //  PokeCentre floor tile
}

std::vector<std::vector<int>> Map::LoadCSV(const std::string& filepath) {
    std::vector<std::vector<int>> data;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open map file at " << filepath << "!\n";
        return data; // Return empty data if it fails
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')) {
            try {
                row.push_back(std::stoi(cell)); 
            } catch (const std::exception& e) {
                // If it hits a stray space or a \r, it catches the error and defaults to 0
                std::cerr << "Bad CSV value '" << cell << "' at row " 
                          << data.size() << ": " << e.what() << "\n";
                row.push_back(0); 
            }
        }
        data.push_back(row);
    }
    return data;
}

void Map::LoadLevel(const std::string& mapName) {
    ClearMap(); 
    
    // 1. Load the two layers
    m_LevelData = LoadCSV(mapName + "_ground.csv");
    m_PropData = LoadCSV(mapName + "_props.csv");

    if (m_LevelData.empty()) {
        std::cout << "Map Ground is EMPTY!" << std::endl;
        return;
    }

    std::vector<std::string> waterPaths = {RESOURCE_DIR "/Water1.png", RESOURCE_DIR "/Water2.png", RESOURCE_DIR "/Water3.png"};
    m_LeaderWater = std::make_shared<Util::Animation>(waterPaths, true, 500, true, 0);
    m_FollowerWater = std::make_shared<Util::Animation>(waterPaths, false, 500, true, 0);
    bool leaderAssigned = false;
    
    // 3. PHASE 1: Build the Ground Tiles
    for (size_t y = 0; y < m_LevelData.size(); y++) {
        for (size_t x = 0; x < m_LevelData[y].size(); x++) {
            int tileID = m_LevelData[y][x];
            
            if (m_TileRegistry.count(tileID) > 0) {
                auto newTile = std::make_shared<Util::GameObject>();
                const TileProperties& props = m_TileRegistry[tileID];

                // Use the config scale and EFFECTIVE_TILE_SIZE
                newTile->m_Transform.scale = {GameConfig::SCALE, GameConfig::SCALE};
                newTile->m_Transform.translation.x = GameConfig::CAMERA_START_X + (x * GameConfig::EFFECTIVE_TILE_SIZE);
                newTile->m_Transform.translation.y = GameConfig::CAMERA_START_Y - (y * GameConfig::EFFECTIVE_TILE_SIZE) + props.yOffset;
                newTile->SetZIndex(props.zIndex);
                
                // Assign the correct image/animation
                if (tileID == 1) { 
                    if (!leaderAssigned) {
                        newTile->SetDrawable(m_LeaderWater); // First tile gets the Leader
                        leaderAssigned = true;
                    } else {
                        newTile->SetDrawable(m_FollowerWater); // The rest follow
                    }
                } else {
                    newTile->SetDrawable(props.texture); // Normal ground tiles
                }

                m_Tiles.push_back(newTile);
            }
        }
    }
  
    // 4. PHASE 2: Spawn the Props!
    if (!m_PropData.empty()) {
        for (size_t y = 0; y < m_PropData.size(); y++) {
            for (size_t x = 0; x < m_PropData[y].size(); x++) {
                int propID = m_PropData[y][x];
                
                // Calculate position using EFFECTIVE_TILE_SIZE
                float worldX = GameConfig::CAMERA_START_X + (x * GameConfig::EFFECTIVE_TILE_SIZE);
                float worldY = GameConfig::CAMERA_START_Y - (y * GameConfig::EFFECTIVE_TILE_SIZE);

                // Use the new GameConfig names instead of magic numbers!
                if (propID == GameConfig::PROP_POKECENTER) { 
                    auto pc = std::make_shared<Prop>(RESOURCE_DIR "/PokeCentre.png", glm::vec2(worldX, worldY));
                    m_Props.push_back(pc);
                }
                else if (propID == GameConfig::PROP_CHURCH) { 
                    auto church = std::make_shared<Prop>(RESOURCE_DIR "/Church.png", glm::vec2(worldX, worldY));
                    m_Props.push_back(church);
                }
                else if (propID == GameConfig::PROP_DOORMAT) { 
                    auto exitDoor = std::make_shared<Prop>(RESOURCE_DIR "/PC_doormat.png", glm::vec2(worldX, worldY));
                    exitDoor->SetZIndex(0.1f);
                    exitDoor->SetDynamicZ(false);
                    m_Props.push_back(exitDoor);
                }
                else if (propID == GameConfig::PROP_PC_DESK) { 
                    // Make sure the image path matches your actual file!
                    auto desk = std::make_shared<Prop>(RESOURCE_DIR "/PCDesk.png", glm::vec2(worldX, worldY));
                    desk->SetZIndex(0.4f); // Sits above the floor, but behind the player if needed
                    desk->SetDynamicZ(false); // Desk doesn't move, so lock it!
                    m_Props.push_back(desk);
                }
                else if (propID == GameConfig::PROP_PC_WALL_LEFT) { 
                    auto leftWall = std::make_shared<Prop>(RESOURCE_DIR "/PCWall2.png", glm::vec2(worldX, worldY));
                    leftWall->SetZIndex(0.3f); // High Z-index so it overlaps the player walking near it
                    leftWall->SetDynamicZ(false);
                    m_Props.push_back(leftWall);
                }
                else if (propID == GameConfig::PROP_PC_WALL_RIGHT) { 
                    auto rightWall = std::make_shared<Prop>(RESOURCE_DIR "/PCWall3.png", glm::vec2(worldX, worldY));
                    rightWall->SetZIndex(0.3f); 
                    rightWall->SetDynamicZ(false);
                    m_Props.push_back(rightWall);
                }
            }
        }
    }
}

void Map::Move(float dx, float dy) {
    for (auto& tile : m_Tiles) {
        tile->m_Transform.translation.x += dx;
        tile->m_Transform.translation.y += dy;
    }
    for (auto& prop : m_Props) {
        prop->m_Transform.translation.x += dx;
        prop->m_Transform.translation.y += dy;
    }
}

void Map::Draw() {
    for (auto& tile : m_Tiles) {
        tile->Draw();
    }
    for (auto& prop : m_Props) {
        prop->Draw(); 
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
    int propID = m_PropData[y][x];
    if (propID == GameConfig::PROP_PC_DESK || 
        propID == GameConfig::PROP_PC_WALL_LEFT || 
        propID == GameConfig::PROP_PC_WALL_RIGHT) {
        return false; // The prop blocks you!
    }
    
    return false; 
}

void Map::Update() {
    // 1. Tell the Follower to copy the Leader's exact page number!
    if (m_LeaderWater && m_FollowerWater) {
        m_FollowerWater->SetCurrentFrame(m_LeaderWater->GetCurrentFrameIndex());
    }

    // 2. Update props
    for (auto& prop : m_Props) {
        prop->Update();
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
    m_Props.clear();
}

int Map::GetPropType(int gridX, int gridY) {
    // Bounds check to prevent crashes
    if (gridY < 0 || gridY >= static_cast<int>(m_PropData.size()) || 
        gridX < 0 || gridX >= static_cast<int>(m_PropData[0].size())) {
        return 0; // 0 means "no prop here"
    }
    
    return m_PropData[gridY][gridX];
}

void Map::WarpTo(int gridX, int gridY) {
    float shiftX = GameConfig::CAMERA_START_X + (gridX * GameConfig::EFFECTIVE_TILE_SIZE);
    float shiftY = GameConfig::CAMERA_START_Y - (gridY * GameConfig::EFFECTIVE_TILE_SIZE);
    Move(-shiftX, -shiftY);
}
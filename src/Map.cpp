#include "Map.hpp"
#include "Prop.hpp"
#include "GameConfig.hpp" // <-- 1. Added our new config file!
#include <fstream>  
#include <sstream>  
#include <iostream> 

Map::Map() { 
    // 2. SETUP THE DICTIONARY
    InitTileRegistry();
    InitNPCRegistry();
    InitPropRegistry();
    // 3. BUILD THE FIRST MAP
    LoadLevel(RESOURCE_DIR "/level");
}

void Map::InitTileRegistry() {
    // ID = { texture, zIndex, yOffset, isWalkable }
    m_TileRegistry[0] = { std::make_shared<Util::Image>(RESOURCE_DIR "/Grass1.png"), 0.0f, 0.0f, true };
    m_TileRegistry[1] = { nullptr, 0.0f, 0.0f, false };
    m_TileRegistry[2] = { std::make_shared<Util::Image>(RESOURCE_DIR "/Dirt1.png"), 0.0f, 0.0f, true };
    m_TileRegistry[3] = { std::make_shared<Util::Image>(RESOURCE_DIR "/PokeCentre.png"), 0.1f, 24.0f, false };
    m_TileRegistry[4] = { m_DirtImage, -0.1f, 0.0f, false }; 
    //m_TileRegistry[41] = { m_DirtImage, 0.0f, 0.0f, false }; 
    //m_TileRegistry[5] = { m_ChurchImage, 0.5f, 0.0f, false };
    m_TileRegistry[6] = { m_DirtImage, 0.0f, 0.0f, false }; // Door (acts as wall)
    //m_TileRegistry[7] = { m_PCDoorImage, 0.1f, 0.0f, false }; // Inside mat (acts as wall)
    //m_TileRegistry[8] = { m_PokeCentreInsideImage, 0.3f, 0.0f, true }; // Inside floor
    m_TileRegistry[9] = { std::make_shared<Util::Image>(RESOURCE_DIR "/PCFloorTile.png"), 0.0f, 0.0f, true }; //  PokeCentre floor tile
    //m_TileRegistry[10] = { m_PCDesk, 0.3f, 0.0f, true };  
    m_TileRegistry[11] = { std::make_shared<Util::Image>(RESOURCE_DIR "/PCWall1.png"), 0.0f, 0.0f, false }; 
    //m_TileRegistry[12] = { m_PCWall2, 0.0f, 0.0f, false }; //  PokeCentre floor tile
    //m_TileRegistry[13] = { m_PCWall3, 0.0f, 0.0f, false }; //  PokeCentre floor tile
}

void Map::InitNPCRegistry() {
    // Map the GameConfig IDs to their specific sprite paths!
    m_NPCRegistry[GameConfig::NPC_NURSE] = RESOURCE_DIR "/Nurse.png";
    //m_NPCRegistry[GameConfig::NPC_OLD_MAN] = RESOURCE_DIR "/oldman.png";
    // m_NPCRegistry[GameConfig::NPC_BOY] = RESOURCE_DIR "/boy.png";
}

void Map::InitPropRegistry() {
    // ID = { texturePath, zIndex, dynamicZ }
    
    // Buildings, to find prop number for map check GameConfig
    m_PropRegistry[GameConfig::PROP_POKECENTER] = { RESOURCE_DIR "/PokeCentre.png", 0.5f, true }; // (Assuming default dynamic true/0.5f)
    m_PropRegistry[GameConfig::PROP_CHURCH] = { RESOURCE_DIR "/Church.png", 0.5f, true };

    // PokeCenter Interiors
    m_PropRegistry[GameConfig::PROP_DOORMAT] = { RESOURCE_DIR "/PC_doormat.png", 0.1f, false };
    m_PropRegistry[GameConfig::PROP_PC_DESK] = { RESOURCE_DIR "/PCDesk1.png", 0.4f, false };
    m_PropRegistry[GameConfig::PROP_PC_WALL_LEFT] = { RESOURCE_DIR "/PCWall2.png", 0.3f, false };
    m_PropRegistry[GameConfig::PROP_PC_WALL_RIGHT] = { RESOURCE_DIR "/PCWall3.png", 0.3f, false };
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
  
    // 4. PHASE 2: Spawn the Props and NPCs!
    if (!m_PropData.empty()) {
        for (size_t y = 0; y < m_PropData.size(); y++) {
            for (size_t x = 0; x < m_PropData[y].size(); x++) {
                int propID = m_PropData[y][x];
                
                // Calculate position using EFFECTIVE_TILE_SIZE
                float worldX = GameConfig::CAMERA_START_X + (x * GameConfig::EFFECTIVE_TILE_SIZE);
                float worldY = GameConfig::CAMERA_START_Y - (y * GameConfig::EFFECTIVE_TILE_SIZE);

                // --- 1. SPAWN NPCs ---
                if (m_NPCRegistry.count(propID) > 0) {
                    // Look up the sprite path for this specific ID
                    std::string spritePath = m_NPCRegistry[propID];
            
                    // Spawn them!
                    float visualOffsetY = 16.0f;
                    auto npc = std::make_shared<NPC>(worldX, worldY + visualOffsetY, spritePath);
                    
                    npc->SetGridPosition(x, y);
                    npc->SetZIndex(0.35f);
                    m_NPCs.push_back(npc);
                }
                
                // --- 2. SPAWN PROPS ---
                if (m_PropRegistry.count(propID) > 0) {
                    // Look up the visual settings for this specific ID
                    const PropProperties& props = m_PropRegistry[propID];
                    
                    // Spawn the Prop using the settings from the dictionary!
                    auto prop = std::make_shared<Prop>(props.texturePath, glm::vec2(worldX, worldY));
                    prop->SetZIndex(props.zIndex);
                    prop->SetDynamicZ(props.dynamicZ);
                    
                    m_Props.push_back(prop);
                }
                
                // (Notice how there are ZERO if / else if statements checking for specific IDs now! 
                // The dictionaries do all the heavy lifting.)
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
    for (auto& npc : m_NPCs) {
        npc->m_Transform.translation.x += dx;
        npc->m_Transform.translation.y += dy; 
    }}

void Map::Draw() {
    for (auto& tile : m_Tiles) {
        tile->Draw();
    }
    for (auto& prop : m_Props) {
        prop->Draw(); 
    }
    for (auto& npc : m_NPCs)   npc->Draw();
}

bool Map::IsWalkable(int x, int y) {
    // 1. Check out of bounds (Fixed the warnings by casting to int!)
    if (x < 0 || x >= (int)m_LevelData[0].size() || y < 0 || y >= (int)m_LevelData.size()) {
        return false;
    }    
    int tileID = m_LevelData[y][x];
    if (m_TileRegistry.count(tileID) == 0) {
        return false; 
    }

    // 2. Check the base tile walkability (Tripwire 1)
    
    if (m_TileRegistry.count(tileID) && !m_TileRegistry[tileID].isWalkable) {
        return false; // The ground itself is solid!
    }

    // 3. Check Props (Tripwire 2)
    int propID = m_PropData[y][x];
    if (propID == GameConfig::PROP_PC_DESK || 
        propID == GameConfig::PROP_PC_WALL_LEFT || 
        propID == GameConfig::PROP_PC_WALL_RIGHT ||
        propID == GameConfig::PROP_INVISIBLE_WALL) {
        return false; // A solid prop is blocking the way!
    }

    // 4. Check NPCs (Tripwire 3)
    // (We will uncomment this once the NPC class is fully implemented!)
    
    for (const auto& npc : m_NPCs) {
        if (npc->GetGridX() == x && npc->GetGridY() == y) {
            return false; // An NPC is standing here!
        }
    }
    


    // If we survived all the tripwires, the tile is clear!
    return true;
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
    for (auto& npc : m_NPCs)   npc->Update(nullptr);
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
    m_NPCs.clear();
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


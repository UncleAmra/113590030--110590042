#include "Map.hpp"
#include "Prop.hpp"
#include "GameConfig.hpp" 
#include "Item.hpp"
#include <fstream>  
#include <sstream>  
#include <iostream> 
#include <algorithm> // just in case you still need it for other logic
#include "ResourceManager.hpp"

//paths to assests
const std::string RES      = std::string(RESOURCE_DIR);
const std::string TILE_DIR = RES + "/tiles/";
const std::string MAP_DIR  = RES + "/maps/";
const std::string PROP_DIR = RES + "/props/";
const std::string NPC_DIR  = RES + "/npcs/";
const std::string DIALOGUE_DIR = RES + "/dialogue/";

Map::Map() { 
    // 2. SETUP THE DICTIONARY
    InitTileRegistry();
    InitNPCRegistry();
    InitPropRegistry();
    InitItemRegistry();
    
    // REMOVED LoadLevel here. We will call it in App::Start() 
    // AFTER the Renderer has been injected!
}

void Map::InitTileRegistry() {
    // ID = { texture, zIndex, yOffset, isWalkable }
    m_TileRegistry[0] = { ResourceManager::GetImageStore().Get(TILE_DIR + "/Grass1.png"), 0.0f, 0.0f, true };
    m_TileRegistry[1] = { nullptr, 0.0f, 0.0f, false };
    m_TileRegistry[2] = { ResourceManager::GetImageStore().Get(TILE_DIR + "/Dirt1.png"), 0.0f, 0.0f, true };
   //3 is used by Pokecentre Prop
    m_TileRegistry[4] = { ResourceManager::GetImageStore().Get(TILE_DIR +"/Concrete.png"), 0.0f, 0.0f, true }; 
   //idk about 5
    m_TileRegistry[6] = { ResourceManager::GetImageStore().Get(TILE_DIR + "/Dirt1.png"), 0.0f, 0.0f, false }; // Door (acts as wall)
    //7 is used by church prop
    m_TileRegistry[9] = { ResourceManager::GetImageStore().Get(TILE_DIR +"/PCFloorTile.png"), 0.0f, 0.0f, true }; //  PokeCentre floor tile  
    m_TileRegistry[11] = { ResourceManager::GetImageStore().Get(TILE_DIR +"/PCWall1.png"), 0.1f, 0.0f, false };
}

void Map::InitNPCRegistry() {
    // ID = { spritePath, visualOffsetY, zIndex, dynamicZ }
    m_NPCRegistry[GameConfig::NPC_NURSE] = NPCProperties{ NPC_DIR + "Nurse", 12.0f, 0.2f, false, DIALOGUE_DIR + "nurse.txt"};
    m_NPCRegistry[GameConfig::NPC_TA1] = NPCProperties{ NPC_DIR + "TA0", -12.0f, 0.8f, true, DIALOGUE_DIR + "ta.txt"};
}

void Map::InitPropRegistry() {
    // ID = { texturePath, zIndex, dynamicZ, isWalkable }
    // Buildings
    m_PropRegistry[GameConfig::PROP_POKECENTER] = { PROP_DIR +  "/PokeCentre.png", 0.0f, 0.8f, true, false }; 
    m_PropRegistry[GameConfig::PROP_CHURCH] = { PROP_DIR + "/Church.png",0.0f, 0.8f, true, false };
    m_PropRegistry[GameConfig::PROP_CHECKPOINT] = { PROP_DIR + "/Checkpoint2.png", 96.0f,0.8f, true, false }; //15
    m_PropRegistry[GameConfig::PROP_CHECKPOINT2] = { PROP_DIR + "/Checkpoint3.png", 96.0f,0.8f, true, false }; //16
    // PokeCenter Interiors
    m_PropRegistry[GameConfig::PROP_DOORMAT] = { PROP_DIR + "/PC_doormat.png", 0.0f,0.1f, false, true }; // WALKABLE! 6 or 7
    m_PropRegistry[GameConfig::PROP_PC_DESK] = { PROP_DIR + "/PCDesk1.png",0.0f, 0.7f, false, false }; //10 I think
    m_PropRegistry[GameConfig::PROP_PC_WALL_LEFT] = { PROP_DIR + "/PCWall2.png", 0.0f,0.3f, false, false }; //12
    m_PropRegistry[GameConfig::PROP_PC_WALL_RIGHT] = { PROP_DIR + "/PCWall3.png",0.0f, 0.3f, false, false }; //13
    //invisible wall
    m_PropRegistry[GameConfig::PROP_INVISIBLE_WALL] = { "",0.0f, 0.0f, false, false}; 
}

void Map::InitItemRegistry() {
    // Item ID = texturePath, name, zIndex
    m_ItemRegistry[50] = { PROP_DIR + "/PokeBall.png", "Potion", ItemCategory::GENERAL, 0.5f }; 
    m_ItemRegistry[51] = { PROP_DIR + "/PokeBall.png", "PokeBall", ItemCategory::POKEBALLS,0.5f }; 
    //m_ItemRegistry[51] = { PROP_DIR + "/PokeBall.png", "PokeBall", ItemCategory::KEY_ITEMS,0.5f }; 


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

// RESTORED ORIGINAL SIGNATURE
void Map::LoadLevel(const std::string& mapName) {
    ClearMap(); 
    
    m_CurrentLevelPath = mapName;
    // 1. Load the two layers
    m_LevelData = LoadCSV(mapName + "_ground.csv");
    m_PropData = LoadCSV(mapName + "_props.csv");

    if (m_LevelData.empty()) {
        std::cout << "Map Ground is EMPTY!" << std::endl;
        return;
    }

    std::vector<std::string> waterPaths = {TILE_DIR + "/Water1.png", TILE_DIR + "/Water2.png", TILE_DIR + "/Water3.png"};
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

                m_Tiles.push_back(newTile);   // Keep for gameplay/collision
                AddToRenderer(newTile);       // USES NEW WEAK_PTR LOGIC
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
                    const NPCProperties& npcProps = m_NPCRegistry[propID];
            
                    auto npc = std::make_shared<NPC>(worldX, worldY + npcProps.visualOffsetY, npcProps.texturePath, npcProps.dialogueFilePath);                  
                    
                    // Use the registry for Z-sorting!
                    npc->SetGridPosition(x, y);
                    npc->SetZIndex(npcProps.zIndex);
                    npc->SetBaseZIndex(npcProps.zIndex);
                    npc->SetDynamicZ(npcProps.dynamicZ); 
                    
                    m_NPCs.push_back(npc);     
                    AddToRenderer(npc);        // USES NEW WEAK_PTR LOGIC
                }
                
                // --- 2. SPAWN PROPS ---
                if (m_PropRegistry.count(propID) > 0) {
                    // Look up the visual settings for this specific ID
                    const PropProperties& props = m_PropRegistry[propID];
                    
                    if (!props.texturePath.empty()) {
                        auto prop = std::make_shared<Prop>(props.texturePath, glm::vec2(worldX, worldY + props.visualOffsetY));
                        prop->SetZIndex(props.zIndex);
                        prop->SetDynamicZ(props.dynamicZ);
                        
                        m_Props.push_back(prop);   
                        AddToRenderer(prop);       // USES NEW WEAK_PTR LOGIC
                    }
                }
                
                // --- 3. SPAWN ITEMS ---
                if (m_ItemRegistry.count(propID) > 0) {
                    // Create a unique ID for this specific tile
                    std::string uniqueID = m_CurrentLevelPath + "_" + std::to_string(x) + "_" + std::to_string(y);

                    // Check if we already looted this spot!
                    if (GameConfig::LootedItems.count(uniqueID) > 0) {
                        m_PropData[y][x] = 0; // Remove it from grid data so we can walk here!
                    } 
                    else {
                        const ItemProperties& itemProps = m_ItemRegistry[propID];
                        
                        if (!itemProps.texturePath.empty()) {
                            auto item = std::make_shared<Item>(itemProps.texturePath, glm::vec2(worldX, worldY), itemProps.name, itemProps.category, x, y);                        
                            m_Items.push_back(item);   
                            AddToRenderer(item);       // USES NEW WEAK_PTR LOGIC
                        }
                    }
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
    for (auto& npc : m_NPCs) {
        npc->m_Transform.translation.x += dx;
        npc->m_Transform.translation.y += dy; 
    }
    for (auto& item : m_Items) {
        item->m_Transform.translation.x += dx;
        item->m_Transform.translation.y += dy;
    }
}

// DRAW IS NOW EMPTY! The Renderer handles this now.
void Map::Draw() {
    // We intentionally leave this empty. 
    // If Map inherits from Util::GameObject, overriding it with an empty body 
    // prevents the Map itself from trying to draw something invalid.
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
    if (m_PropRegistry.count(propID) > 0 && !m_PropRegistry[propID].isWalkable) {
        return false; // A solid prop is blocking the way!
    }
    
    // 3.5 Check Items
    if (m_ItemRegistry.count(propID) > 0) {
        return false; // Items act as solid walls until you pick them up!
    }

    // 4. Check NPCs (Tripwire 3)
    for (const auto& npc : m_NPCs) {
        if (npc->GetGridX() == x && npc->GetGridY() == y) {
            return false; // An NPC is standing here!
        }
    }
    
    // If we survived all the tripwires, the tile is clear!
    return true;
}

// Change the signature back to empty
void Map::Update() {
    if (m_LeaderWater && m_FollowerWater) {
        m_FollowerWater->SetCurrentFrame(m_LeaderWater->GetCurrentFrameIndex());
    }

    for (auto& prop : m_Props) {
        prop->Update();
    }
    
    // Pass shared_from_this() to the NPCs!
    for (auto& npc : m_NPCs) {
        npc->Update(shared_from_this());
    }
}

int Map::GetTileType(int gridX, int gridY) {
    if (gridY < 0 || gridY >= static_cast<int>(m_LevelData.size()) || 
        gridX < 0 || gridX >= static_cast<int>(m_LevelData[0].size())) {
        return -1; 
    }
    return m_LevelData[gridY][gridX];
}

void Map::SetRenderer(std::weak_ptr<Util::Renderer> renderer) { 
    m_Renderer = renderer; 
}

void Map::AddToRenderer(std::shared_ptr<Util::GameObject> obj) {
    if (auto r = m_Renderer.lock()) {
        r->AddChild(obj);
    }
}

void Map::ClearMap() {
    if (auto r = m_Renderer.lock()) {
        for (auto& tile : m_Tiles) r->RemoveChild(tile);
        for (auto& prop : m_Props) r->RemoveChild(prop);
        for (auto& npc  : m_NPCs)  r->RemoveChild(npc);
        for (auto& item : m_Items) r->RemoveChild(item);
    }
    m_Tiles.clear();
    m_LevelData.clear();
    m_PropData.clear(); 
    m_Props.clear();
    m_NPCs.clear();
    m_Items.clear();
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

std::shared_ptr<NPC> Map::GetNPCAt(int gridX, int gridY) {
    for (auto& npc : m_NPCs) {
        if (npc->GetGridX() == gridX && npc->GetGridY() == gridY) {
            return npc;
        }
    }
    return nullptr;
}

std::string Map::CollectItemAt(int gridX, int gridY, Character& player) {
    int propID = GetPropType(gridX, gridY);

    if (m_ItemRegistry.count(propID) > 0) {
        // 1. Pull the name AND category straight from your registry
        std::string itemName = m_ItemRegistry[propID].name;
        ItemCategory itemCat = m_ItemRegistry[propID].category; 

        // 2. Pass the registry category into the player's inventory
        player.AddItem(itemName, itemCat, 1);
        m_PropData[gridY][gridX] = 0;

        // 3. Find the actual item in the world and mark it as collected
        for (auto& item : m_Items) {
            if (item->GetGridX() == gridX && item->GetGridY() == gridY && !item->IsCollected()) {
                item->Collect();
                break;
            }
        }
        
        // 4. Write to the blacklist
        std::string uniqueID = m_CurrentLevelPath + "_" + std::to_string(gridX) + "_" + std::to_string(gridY);
        GameConfig::LootedItems.insert(uniqueID);
        
        return itemName; 
    }
    return ""; 
}
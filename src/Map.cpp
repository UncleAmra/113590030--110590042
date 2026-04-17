#include <algorithm>
#include "Map.hpp"
#include "Prop.hpp"
#include "GameConfig.hpp" 
#include "Item.hpp"
#include <fstream>  
#include <sstream>  
#include <iostream> 
#include "TrainerDatabase.hpp"
#include "ResourceManager.hpp"

//paths to assests
const std::string RES      = std::string(RESOURCE_DIR);
const std::string TILE_DIR = RES + "/tiles/";
const std::string MAP_DIR  = RES + "/maps/";
const std::string PROP_DIR = RES + "/props/";
const std::string NPC_DIR  = RES + "/npcs/";
const std::string DIALOGUE_DIR = RES + "/dialogue/";

Map::Map() { 
    InitTileRegistry();
    InitNPCRegistry();
    InitPropRegistry();
    InitItemRegistry();
}

void Map::InitTileRegistry() {
    // ID = { texture, zIndex, yOffset, isWalkable }
    m_TileRegistry[GameConfig::TILE_GRASS]          ={ ResourceManager::GetImageStore().Get(TILE_DIR + "/Grass1.png"), 0.0f, 0.0f, true };
    m_TileRegistry[GameConfig::TILE_WATER_SOLID]    ={ nullptr, 0.0f, 0.0f, false };
    m_TileRegistry[GameConfig::TILE_DIRT]           ={ ResourceManager::GetImageStore().Get(TILE_DIR + "/Dirt1.png"),     0.0f, 0.0f, true };
    m_TileRegistry[GameConfig::TILE_SAND]           ={ ResourceManager::GetImageStore().Get(TILE_DIR + "/Sand.png"),      0.0f, 0.0f, true };
    m_TileRegistry[GameConfig::TILE_CONCRETE]       ={ ResourceManager::GetImageStore().Get(TILE_DIR +"/Concrete.png"),   0.0f, 0.0f, true }; 
    m_TileRegistry[GameConfig::TILE_DOOR]           ={ ResourceManager::GetImageStore().Get(TILE_DIR + "/Dirt1.png"),     0.0f, 0.0f, false }; 
    m_TileRegistry[GameConfig::TILE_PC_FLOOR]       ={ ResourceManager::GetImageStore().Get(TILE_DIR +"/PCFloorTile.png"), 0.0f, 0.0f, true }; 
    m_TileRegistry[GameConfig::TILE_PM_FLOOR]       ={ ResourceManager::GetImageStore().Get(TILE_DIR +"/PokeMartTile.png"), 0.0f, 0.0f, true }; 
    m_TileRegistry[GameConfig::TILE_PC_WALL]        ={ ResourceManager::GetImageStore().Get(TILE_DIR +"/PCWall1.png"),      0.1f, 0.0f, false };
    m_TileRegistry[GameConfig::TILE_INSIDE_CHURCH]  ={ ResourceManager::GetImageStore().Get(TILE_DIR +"/church_inside.png"), 0.1f, 0.0f, true };
}
void Map::InitNPCRegistry() {
    // ID = { spritePath, visualOffsetY, zIndex, dynamicZ, NPCActionType, ActionData }
    m_NPCRegistry[GameConfig::NPC_NURSE]        =NPCProperties{ NPC_DIR + "Nurse", 12.0f, 0.2f, false, DIALOGUE_DIR + "nurse.txt", NPCAction::HEAL, ""};
    m_NPCRegistry[GameConfig::NPC_TA1]          =NPCProperties{ NPC_DIR + "TA0", -12.0f, 0.5f, true, DIALOGUE_DIR + "ta.txt", NPCAction::BATTLE, "Trainer_TA"};
    m_NPCRegistry[GameConfig::SHOP_KEEPER]      =NPCProperties{ NPC_DIR + "ShopKeeper", -12.0f, 0.5f, true, DIALOGUE_DIR + "ta.txt", NPCAction::SHOP, "Mart_Potions"};
}
void Map::InitPropRegistry() {
    // FORMAT: { {"Frame1", "Frame2", ...}, zIndex, dynamicZ, isWalkable, offsetX, offsetY }
    m_PropRegistry[GameConfig::PROP_INVISIBLE_DOOR]    ={ {}, 0.0f, false, false,  0.0f, 0.0f }; 
    m_PropRegistry[GameConfig::PROP_INVISIBLE_WALL]    ={ {}, 0.0f, false, false, 0.0f, 0.0f }; 
    m_PropRegistry[GameConfig::PROP_INTERACTABLE_WALL] ={ {}, 0.0f, false, false, 0.0f, 0.0f }; 
    m_PropRegistry[GameConfig::PROP_POKECENTER]        ={ {PROP_DIR + "/PokeCentre.png"},   0.8f, true, false, 0.0f, 0.0f }; 
    m_PropRegistry[GameConfig::PROP_POKEMONGYM]        ={ {PROP_DIR + "/PokemonGym.png"},   0.8f, true, false, 0.0f, 12.0f }; 
    m_PropRegistry[GameConfig::PROP_CHURCH]            ={ {PROP_DIR + "/Church.png"},       0.8f, true, false, 0.0f, 0.0f };
    m_PropRegistry[GameConfig::WOODEN_HOUSE]           ={ {PROP_DIR + "/wood_house.png"},  0.8f, true, false, 0.0f, 0.0f };
    m_PropRegistry[GameConfig::POKEMART]               ={ {PROP_DIR + "/PokeMart.png"},     0.8f, true, false, 24.0f, 0.0f };
    m_PropRegistry[GameConfig::PROP_CHECKPOINT]        ={ {PROP_DIR + "/Checkpoint2.png"}, 0.8f, true, false, 0.0f, 96.0f }; 
    m_PropRegistry[GameConfig::PROP_CHECKPOINT2]       ={ {PROP_DIR + "/Checkpoint3.png"}, 0.8f, true, false, 0.0f, 96.0f }; 
    m_PropRegistry[GameConfig::PROP_DOORMAT]           ={ {PROP_DIR + "/PC_doormat.png"},   0.1f, false, true,  0.0f, -20.0f }; 
    m_PropRegistry[GameConfig::PROP_PC_DESK]           ={ {PROP_DIR + "/PCDesk1.png"},      0.4f, false, false, 0.0f, 0.0f }; 
    m_PropRegistry[GameConfig::PROP_PM_DESK]           ={ {PROP_DIR + "/PokeMartDesk.png"}, 0.4f, false, false, 24.0f, 0.0f }; 
    m_PropRegistry[GameConfig::PROP_PC_WALL_LEFT]      ={ {PROP_DIR + "/PCWall2.png"},      0.3f, false, false, 0.0f, 0.0f }; 
    m_PropRegistry[GameConfig::PROP_PC_WALL_RIGHT]     ={ {PROP_DIR + "/PCWall3.png"},      0.3f, false, false, 0.0f, 0.0f }; 
    m_PropRegistry[GameConfig::PROP_TREE]              ={ {PROP_DIR + "/Tree.png"}, 0.8f, true, true, 20.0f, -16.0f }; 
    // 6. INTERACTIVE PROPS (2 Textures)
    m_PropRegistry[GameConfig::PROP_TALLGRASS] = { 
        { PROP_DIR + "/TallGrass2.png" , PROP_DIR + "/TallGrass3.png", PROP_DIR + "/TallGrass4.png" }, // Normal, then Flattened!
        1.0f, true, true, 0.0f, 0.0f 
    }; 
    m_PropRegistry[GameConfig::POKEMART_SIGN] = {
        { PROP_DIR + "/PokeMartSign1.png",
        PROP_DIR + "/PokeMartSign2.png",
        PROP_DIR + "/PokeMartSign3.png",
        PROP_DIR + "/PokeMartSign4.png" },
        0.9f,    // zIndex
        true,    // dynamicZ
        false,   // isWalkable
        16.0f,    // offsetX
        32.0f,    // offsetY
        PropAnimMode::LOOP,  // animMode
        30     // frameDelay — lower = faster
    };
}

void Map::InitItemRegistry() {
    // Item ID = texturePath, name, category, zIndex
    m_ItemRegistry[GameConfig::ITEM_POTION]     = { PROP_DIR + "/PokeBall.png", "Potion", ItemCategory::GENERAL, 0.5f }; 
    m_ItemRegistry[GameConfig::ITEM_POKEBALL]   = { PROP_DIR + "/PokeBall.png", "Pokeball", ItemCategory::POKEBALLS, 0.5f }; 
}

std::vector<std::vector<int>> Map::LoadCSV(const std::string& filepath) {
    std::vector<std::vector<int>> data;
    std::ifstream file(filepath);
    if (!file.is_open()) { std::cerr << "ERROR: Could not open map file at " << filepath << "!\n";
        return data; // Return empty data if it fails
    }

    
    std::string line;
    while (std::getline(file, line)) {
        std::vector      <int> row;
        std::stringstream ss(line);
        std::string       cell;
        while (std::getline(ss, cell, ',')) {
            try {
                row.push_back(std::stoi(cell)); 
            } catch (const std::exception& e) {
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
    m_LevelData = LoadCSV(mapName + "_ground.csv");// 1. Load the two layers
    m_PropData  = LoadCSV(mapName + "_props.csv");

    if (m_LevelData.empty()) {
        std::cout << "Map Ground is EMPTY!" << std::endl;
        return;
    }

    std::vector<std::string> 
    waterPaths      = {TILE_DIR + "/Water1.png", TILE_DIR + "/Water2.png", TILE_DIR + "/Water3.png"};
    m_LeaderWater   = std::make_shared<Util::Animation>(waterPaths, true, 500, true, 0);
    m_FollowerWater = std::make_shared<Util::Animation>(waterPaths, false, 500, true, 0);
    bool leaderAssigned = false;
    
    // 3. PHASE 1: Build the Ground Tiles
    for     (size_t y = 0; y < m_LevelData.size(); y++) {
        for (size_t x = 0; x < m_LevelData[y].size(); x++) {
            int tileID = m_LevelData[y][x];
            
            if (m_TileRegistry.count(tileID) > 0) {
                auto newTile = std::make_shared<Util::GameObject>();
                const TileProperties& props = m_TileRegistry[tileID];

                // Use the config scale and EFFECTIVE_TILE_SIZE
                newTile->m_Transform.scale          = {GameConfig::SCALE, GameConfig::SCALE};
                newTile->m_Transform.translation.x  =  GameConfig::CAMERA_START_X + (x * GameConfig::EFFECTIVE_TILE_SIZE);
                newTile->m_Transform.translation.y  =  GameConfig::CAMERA_START_Y - (y * GameConfig::EFFECTIVE_TILE_SIZE) + props.yOffset;
                newTile->SetZIndex(props.zIndex);
                
                // Assign the correct image/animation
                if (tileID == 1) { 
                    if (!leaderAssigned) {
                        newTile->SetDrawable(m_LeaderWater);        // First tile gets the Leader
                        leaderAssigned = true;}
                    else {newTile->SetDrawable(m_FollowerWater);} } // The rest follow   
                else {newTile->SetDrawable(props.texture);}         // Normal ground tiles
                m_Tiles.push_back(newTile);                         // Keep for gameplay/collision
                AddToRenderer(newTile);                             // USES NEW WEAK_PTR LOGIC
            }
        }
    }
  
    // 4. PHASE 2: Spawn the Props and NPCs!
    if (!m_PropData.empty()) {
        for (size_t y = 0; y < m_PropData.size(); y++) {
            for (size_t x = 0; x < m_PropData[y].size(); x++) {
                int propID = m_PropData[y][x];                      // Calculate position using EFFECTIVE_TILE_SIZE
                float worldX = GameConfig::CAMERA_START_X + (x * GameConfig::EFFECTIVE_TILE_SIZE);
                float worldY = GameConfig::CAMERA_START_Y - (y * GameConfig::EFFECTIVE_TILE_SIZE);
                                                                    //  NPC
                if (m_NPCRegistry.count(propID) > 0) {
                        const NPCProperties& npcProps = m_NPCRegistry[propID];          
                        auto npc = std::make_shared<NPC>(worldX, worldY + npcProps.visualOffsetY, npcProps.texturePath, npcProps.dialogueFilePath,"","");            
                        npc->SetGridPosition(x, y);
                        npc->SetZIndex(npcProps.zIndex);
                        npc->SetBaseZIndex(npcProps.zIndex);
                        npc->SetDynamicZ(npcProps.dynamicZ);
                        npc->SetAction(npcProps.actionType, npcProps.actionData); 
                        
                        // Assign Pokemon team if they are a trainer
                        if (npcProps.actionType == NPCAction::BATTLE) {
                            auto loadedParty = TrainerDatabase::CreateTrainerParty(npcProps.actionData);
                            for (const auto& p : loadedParty) {
                                npc->GetParty().push_back(p);
                            }
                        }

                        m_NPCs.push_back(npc);     
                        AddToRenderer(npc);        
                    }

                //Props
                if (m_PropRegistry.count(propID) > 0) {
                    const PropProperties& props = m_PropRegistry[propID];
                    glm::vec2 spawnPos(worldX + props.visualOffsetX, worldY + props.visualOffsetY);
                    auto prop = std::make_shared<Prop>(
                        props.texturePaths, 
                        spawnPos, 
                        3.0f,               
                        props.zIndex, 
                        x,                  
                        y                   
                    );

                    prop->SetDynamicZ   (props.dynamicZ);
                    prop->SetZIndex     (props.zIndex);     
                    prop->SetAnimMode   (props.animMode, props.animFrameDelay); 
                    m_Props.push_back   (prop);   
                    if (!props.texturePaths.empty()) {AddToRenderer(prop);} //ONLY add it to the visual engine if it actually has images!
                }
                
                // spawn items
                if (m_ItemRegistry.count(propID) > 0) {
                    std::string uniqueID = m_CurrentLevelPath + "_" + std::to_string(x) + "_" + std::to_string(y);
                    if (GameConfig::LootedItems.count(uniqueID) > 0) {      // Check if we already looted this spot!
                        m_PropData[y][x] = 0;}                               // Remove it from grid data so we can walk here! 
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
        //return m_OutOfBoundsPropID;
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

void Map::UpdateSteppedProps(int playerGridX, int playerGridY) {
    for (auto& prop : m_Props) {
        // Skip props that don't use the grid system (like buildings)
        if (prop->GetGridX() == -1 || prop->GetGridY() == -1) continue;

        // If the player's coordinates match the prop's coordinates, squish it!
        if (prop->GetGridX() == playerGridX && prop->GetGridY() == playerGridY) {
            prop->SetSteppedOn(true); 
        } else {
            prop->SetSteppedOn(false); 
        }
    }
}

void Map::SetVisible(bool visible) {
    // This looks very similar to your ClearMap loop, but instead of removing 
    // them from the renderer or deleting them, it just toggles their visibility!
    
    for (auto& tile : m_Tiles) {
        if (tile) tile->SetVisible(visible);
    }
    for (auto& prop : m_Props) {
        if (prop) prop->SetVisible(visible);
    }
    for (auto& npc : m_NPCs) {
        if (npc) npc->SetVisible(visible);
    }
    for (auto& item : m_Items) {
        if (item) item->SetVisible(visible);
    }
}
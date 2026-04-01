#include "App.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Character.hpp"
#include "Item.hpp"
#include "SaveSystem.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Text.hpp"
#include <iostream>
#include "StartMenu.hpp"
#include "InventoryMenu.hpp"
#include "ResourceManager.hpp"

const std::string RES      = std::string(RESOURCE_DIR);
const std::string MAP_DIR  = RES + "/maps/";

void App::Start() {
    LOG_TRACE("Start");
    
    // ==========================================
    // 1. INITIALIZE ARCHITECTURE
    // ==========================================
    m_Renderer = std::make_shared<Util::Renderer>();
    m_Map = std::make_shared<Map>();
    m_Map->SetRenderer(m_Renderer); 
    
    m_Character = std::make_shared<Player>(0.0f, 0.0f);
    m_Renderer->AddChild(m_Character); 

    // ==========================================
    // 2. LOAD GAME OR START FRESH
    // ==========================================
    SaveSystem::GameState loadedState;
    
    if (SaveSystem::LoadGame(loadedState)) {
        GameConfig::LootedItems = loadedState.lootedItems;
        m_Map->LoadLevel(loadedState.mapPath);
        
        m_Character->SetGridPosition(loadedState.gridX, loadedState.gridY);
        m_Map->WarpTo(loadedState.gridX, loadedState.gridY);
        m_Character->SetDirection(static_cast<Character::Direction>(loadedState.direction));
        m_Character->SetInventory(loadedState.inventory);
    } else {
        GameConfig::LootedItems.clear(); 
        m_Map->LoadLevel(MAP_DIR + "level"); 
        m_Character->SetGridPosition(10, 10); 
        m_Map->WarpTo(10, 10);
    }
    
    // ==========================================
    // 3. BUILD THE DIALOGUE UI
    // ==========================================
    m_DialogueBoxUI = std::make_shared<Util::GameObject>();
    auto boxImage = ResourceManager::GetImageStore().Get(RESOURCE_DIR "/UI/BWTextBox3.png");    
    m_DialogueBoxUI->SetDrawable(boxImage);
    m_DialogueBoxUI->SetZIndex(9.0f); 
    m_DialogueBoxUI->SetVisible(false);
    m_DialogueBoxUI->m_Transform.scale = {1.0f, 1.0f};          
    m_DialogueBoxUI->m_Transform.translation = {0.0f, -288.0f};

    m_DialogueUI = std::make_shared<Util::GameObject>();
    m_DialogueText = std::make_shared<Util::Text>(
        RESOURCE_DIR "/Fonts/micross.ttf", 
        24,                              
        "...",                           
        Util::Color(50, 50, 50) 
    );
    m_DialogueUI->SetDrawable(m_DialogueText);
    m_DialogueUI->SetZIndex(10.0f);  
    m_DialogueUI->SetVisible(false);
    m_DialogueUI->m_Transform.translation = {-600.0f, -260.0f};

    // ==========================================
    // 4. BUILD THE INVENTORY UI
    // ==========================================
    m_InventoryMenu = std::make_shared<InventoryMenu>(m_Renderer);
    
    // ==========================================
    // 5. BUILD THE START MENU UI
    // ==========================================
    m_StartMenu = std::make_shared<StartMenu>(m_Renderer);
    
    m_Renderer->AddChild(m_DialogueBoxUI);
    m_Renderer->AddChild(m_DialogueUI);
    // Note: InventoryBoxUI and InventoryTextUI are no longer added here! 
    // InventoryMenu handles its own rendering registration internally.

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    // Handle quitting
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    // Handle Saving 
    if (Util::Input::IsKeyDown(Util::Keycode::F) && !m_IsInDialogue && m_MenuState == MenuState::CLOSED) {
        SaveSystem::GameState current;
        current.mapPath = m_Map->GetCurrentLevelPath(); 
        current.gridX = m_Character->GetGridX();
        current.gridY = m_Character->GetGridY();
        current.direction = static_cast<int>(m_Character->GetFacingDirection());
        current.inventory = m_Character->GetInventory();
        current.lootedItems = GameConfig::LootedItems;
        SaveSystem::SaveGame(current);
    }

    // ==========================================
    // 1. MENU TOGGLE LOGIC ('I' Key)
    // ==========================================
    if (Util::Input::IsKeyDown(Util::Keycode::I) && !m_IsInDialogue && !m_Character->IsMoving()) {
        if (m_MenuState == MenuState::CLOSED) {
            m_MenuState = MenuState::START_MENU;
            m_Character->StopMoving(); 
            
            m_StartMenu->SetVisible(true);
            LOG_TRACE("Start Menu Opened");
        } 
        else if (m_MenuState == MenuState::START_MENU) {
            m_MenuState = MenuState::CLOSED;
            m_StartMenu->SetVisible(false);
            LOG_TRACE("Start Menu Closed");
        }
    }

    // ==========================================
    // 2. STATE MACHINE
    // ==========================================
    if (m_IsInDialogue) {
        // --- DIALOGUE STATE ---
        if (Util::Input::IsKeyDown(Util::Keycode::Z)) {
            if (!m_CurrentDialogueLines.empty() && m_CurrentDialogueIndex < m_CurrentDialogueLines.size() - 1) {
                m_CurrentDialogueIndex++;
                
                // Set the new text
                m_DialogueText->SetText(m_CurrentDialogueLines[m_CurrentDialogueIndex]);
                
                // X-SHIFT ALIGNMENT
                float textHalfWidth = m_DialogueText->GetSize().x / 2.0f;
                m_DialogueUI->m_Transform.translation.x = -600.0f + textHalfWidth; 
            } 
            else {
                m_IsInDialogue = false;
                m_DialogueBoxUI->SetVisible(false);
                m_DialogueUI->SetVisible(false);
            }
        }
    }
    else if (m_MenuState == MenuState::START_MENU) {
        // --- START MENU STATE ---
        StartMenu::MenuOption selection = m_StartMenu->Update();
        
        if (selection == StartMenu::MenuOption::BAG) {
            LOG_TRACE("Selected: BAG");
            m_MenuState = MenuState::BAG_MAIN; 
            m_StartMenu->SetVisible(false);
            
            // Fetch inventory map and convert it into a vector list for the menu
            auto inventory = m_Character->GetInventory();
            std::vector<std::pair<std::string, int>> inventoryList;
            
            for (const auto& [itemName, qty] : inventory) {
                inventoryList.push_back({itemName, qty});
            }
            
            // Pass the vector list to the menu!
            m_InventoryMenu->Show(inventoryList);

        } else if (selection == StartMenu::MenuOption::SAVE) {
            LOG_TRACE("Selected: SAVE");
            
            SaveSystem::GameState current;
            current.mapPath = m_Map->GetCurrentLevelPath(); 
            current.gridX = m_Character->GetGridX();
            current.gridY = m_Character->GetGridY();
            current.direction = static_cast<int>(m_Character->GetFacingDirection());
            current.inventory = m_Character->GetInventory();
            current.lootedItems = GameConfig::LootedItems;
            SaveSystem::SaveGame(current);
            
            m_MenuState = MenuState::CLOSED;
            m_StartMenu->SetVisible(false);
            LOG_TRACE("Game Saved Successfully!");

        } else if (selection == StartMenu::MenuOption::EXIT) {
            LOG_TRACE("Selected: EXIT");
            m_CurrentState = State::END; 
        }
        
        m_Renderer->Update();
        return; 
    }
    else if (m_MenuState == MenuState::BAG_MAIN) {
        // --- BAG INVENTORY STATE ---
        // Let the InventoryMenu handle its own input. If it returns true, we exit.
        if (m_InventoryMenu->Update()) {
            LOG_TRACE("Exiting Bag...");
            m_MenuState = MenuState::START_MENU; 
            
            m_InventoryMenu->Hide();
            m_StartMenu->SetVisible(true);
        }
        
        m_Renderer->Update();
        return;
    }
    else {
        // --- NORMAL GAME STATE ---
        
        // A. INTERACTION ('Z' KEY)
        if (Util::Input::IsKeyDown(Util::Keycode::Z) && !m_Character->IsMoving()) {
            int checkX = m_Character->GetGridX();
            int checkY = m_Character->GetGridY();

            Character::Direction facing = m_Character->GetFacingDirection();
            if (facing == Character::Direction::UP)    checkY -= 1;
            if (facing == Character::Direction::DOWN)  checkY += 1;
            if (facing == Character::Direction::LEFT)  checkX -= 1;
            if (facing == Character::Direction::RIGHT) checkX += 1;

            auto targetNPC = m_Map->GetNPCAt(checkX, checkY);
            
            if (targetNPC) {
                m_Character->StopMoving();
                m_IsInDialogue = true;
                
                m_DialogueBoxUI->SetVisible(true);
                m_DialogueUI->SetVisible(true);

                Character::Direction playerDir = m_Character->GetFacingDirection();
                if (playerDir == Character::Direction::UP)         targetNPC->SetDirection(Character::Direction::DOWN);
                else if (playerDir == Character::Direction::DOWN)  targetNPC->SetDirection(Character::Direction::UP);
                else if (playerDir == Character::Direction::LEFT)  targetNPC->SetDirection(Character::Direction::RIGHT);
                else if (playerDir == Character::Direction::RIGHT) targetNPC->SetDirection(Character::Direction::LEFT);

                m_CurrentDialogueLines = targetNPC->Interact();
                m_CurrentDialogueIndex = 0; 

                if (!m_CurrentDialogueLines.empty()) {
                    m_DialogueText->SetText(m_CurrentDialogueLines[m_CurrentDialogueIndex]);
                    
                    // X-SHIFT ALIGNMENT
                    float textHalfWidth = m_DialogueText->GetSize().x / 2.0f;
                    m_DialogueUI->m_Transform.translation.x = -600.0f + textHalfWidth;
                }
            }
            else {
                std::string collectedItem = m_Map->CollectItemAt(checkX, checkY, *m_Character);
                
                if (!collectedItem.empty()) {
                    m_Character->StopMoving();
                    m_IsInDialogue = true; 
                    
                    m_DialogueBoxUI->SetVisible(true);
                    m_DialogueUI->SetVisible(true);
                    
                    m_CurrentDialogueLines = { "Obtained 1x " + collectedItem + "!" };
                    m_CurrentDialogueIndex = 0;
                    m_DialogueText->SetText(m_CurrentDialogueLines[m_CurrentDialogueIndex]);
                    
                    // X-SHIFT ALIGNMENT
                    float textHalfWidth = m_DialogueText->GetSize().x / 2.0f;
                    m_DialogueUI->m_Transform.translation.x = -600.0f + textHalfWidth;
                }
            }
        }

        // B. MOVEMENT
        glm::vec2 movement = m_Character->Update(m_Map);
        m_Map->Move(-movement.x, -movement.y);

        // C. DOORS AND WARPING
        if (m_Character->HasHitDoor()) {
            std::string currentMap = m_Map->GetCurrentLevelPath();
            int doorX = m_Character->GetGridX();
            int doorY = m_Character->GetGridY();

            std::string doorKey = currentMap + "_" + std::to_string(doorX) + "_" + std::to_string(doorY);

            auto it = GameConfig::DoorRouting.find(doorKey);
            
            if (it != GameConfig::DoorRouting.end()) {
                GameConfig::WarpDestination dest = it->second;
                LOG_TRACE("Taking door to: {}", dest.levelPath);
                
                m_Map->LoadLevel(dest.levelPath); 
                m_Character->SetGridPosition(dest.spawnX, dest.spawnY);
                m_Map->WarpTo(dest.spawnX, dest.spawnY);
                m_Character->StopMoving();
                m_Character->ClearDoorFlag(); 
            } else {
                LOG_ERROR("Unregistered door at: {}", doorKey);
                m_Character->ClearDoorFlag(); 
            }
        }
    }

    // ==========================================
    // 3. GENERAL UPDATES & DRAWING
    // ==========================================
    m_Map->Update();
    m_Renderer->Update();
}

void App::End() {
    LOG_TRACE("End");
}
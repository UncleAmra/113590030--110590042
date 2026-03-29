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
const std::string RES      = std::string(RESOURCE_DIR);
const std::string MAP_DIR = RES + "/maps/";

void App::Start() {
    LOG_TRACE("Start");
    SaveSystem::GameState loadedState;
    
    if (SaveSystem::LoadGame(loadedState)) {
        // 1. SET THE BLACKLIST FIRST! (Before the Map constructor runs)
        GameConfig::LootedItems = loadedState.lootedItems;
        
        // 2. NOW CREATE AND LOAD THE MAP
        m_Map = std::make_shared<Map>();
        m_Map->LoadLevel(loadedState.mapPath);
        
        // 3. SET PLAYER STATE & INVENTORY
        m_Character.SetGridPosition(loadedState.gridX, loadedState.gridY);
        m_Map->WarpTo(loadedState.gridX, loadedState.gridY);
        m_Character.SetDirection(static_cast<Character::Direction>(loadedState.direction));
        m_Character.SetInventory(loadedState.inventory);
    } else {
        // DEFAULT START (If no save file exists)
        GameConfig::LootedItems.clear(); // Ensure blacklist is empty for a new game
        
        m_Map = std::make_shared<Map>();
        
        // FIX: Removed the extra slash before "level" to fix the door bug!
        m_Map->LoadLevel(MAP_DIR + "level"); 
        
        m_Character.SetGridPosition(10, 10); 
        m_Map->WarpTo(10, 10);
    }
    
    // ==========================================
    // BUILD THE DIALOGUE UI
    // ==========================================
    m_DialogueBoxUI = std::make_shared<Util::GameObject>();
    auto boxImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Fonts/BWTextBox2.png"); 
    m_DialogueBoxUI->SetDrawable(boxImage);
    m_DialogueBoxUI->SetZIndex(9.0f); // Under the text, over the game
    m_DialogueBoxUI->SetVisible(false);

    // Move it to the bottom of the screen! 
    m_DialogueBoxUI->m_Transform.scale = {1.0f, 1.0f};          // Shrink the massive image
    m_DialogueBoxUI->m_Transform.translation = {0.0f, -288.0f};

    // 2. The Text
    m_DialogueUI = std::make_shared<Util::GameObject>();
    m_DialogueText = std::make_shared<Util::Text>(
        RESOURCE_DIR "/Fonts/micross.ttf", 
        24,                              
        "...",                           
        Util::Color(50, 50, 50) // Dark Grey
    );
    m_DialogueUI->SetDrawable(m_DialogueText);
    m_DialogueUI->SetZIndex(10.0f);  // On top of the box!
    m_DialogueUI->SetVisible(false);

    // Position the text slightly offset from the center of the box so it starts top-left
    m_DialogueUI->m_Transform.translation = {-450.0f, -260.0f};
    
    m_CurrentState = State::UPDATE;
}
void App::Update() {
    // Handle quitting
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
    if (Util::Input::IsKeyDown(Util::Keycode::F) && !m_IsInDialogue) {
        SaveSystem::GameState current;
        current.mapPath = m_Map->GetCurrentLevelPath(); // Ensure your Map class has this getter!
        current.gridX = m_Character.GetGridX();
        current.gridY = m_Character.GetGridY();
        current.direction = static_cast<int>(m_Character.GetFacingDirection());
        current.inventory = m_Character.GetInventory();
        current.lootedItems = GameConfig::LootedItems;
        SaveSystem::SaveGame(current);
    }
    // ==========================================
    // 1. STATE MACHINE (Input & Logic)
    // ==========================================
    if (m_IsInDialogue) {
        if (Util::Input::IsKeyDown(Util::Keycode::Z)) {
            // Guard against empty vectors AND check the index safely
            if (!m_CurrentDialogueLines.empty() && m_CurrentDialogueIndex < m_CurrentDialogueLines.size() - 1) {
                m_CurrentDialogueIndex++;
                m_DialogueText->SetText(m_CurrentDialogueLines[m_CurrentDialogueIndex]);
            } 
            else {
                m_IsInDialogue = false;
                m_DialogueBoxUI->SetVisible(false);
                m_DialogueUI->SetVisible(false);
                
            }
        }
    }
    else {
        // --- NORMAL GAME STATE ---

        // 1a. Check for Interaction (Pressing 'Z' to talk)
        if (Util::Input::IsKeyDown(Util::Keycode::Z) && !m_Character.IsMoving()) {
            
            // 1. Get the player's current grid position
            int checkX = m_Character.GetGridX();
            int checkY = m_Character.GetGridY();

            // 2. Adjust the coordinates based on which way the player is facing
            Character::Direction facing = m_Character.GetFacingDirection();
            if (facing == Character::Direction::UP)    checkY -= 1;
            if (facing == Character::Direction::DOWN)  checkY += 1;
            if (facing == Character::Direction::LEFT)  checkX -= 1;
            if (facing == Character::Direction::RIGHT) checkX += 1;

            // 3. Ask the map if an NPC is there
            auto targetNPC = m_Map->GetNPCAt(checkX, checkY);
            
            if (targetNPC) {
                m_Character.StopMoving();
                m_IsInDialogue = true;
                m_DialogueBoxUI->SetVisible(true);
                m_DialogueUI->SetVisible(true);

                // ==========================================
                // MAKE THE NPC FACE THE PLAYER!
                // ==========================================
                Character::Direction playerDir = m_Character.GetFacingDirection();
                if (playerDir == Character::Direction::UP) {
                    targetNPC->SetDirection(Character::Direction::DOWN);
                } 
                else if (playerDir == Character::Direction::DOWN) {
                    targetNPC->SetDirection(Character::Direction::UP);
                } 
                else if (playerDir == Character::Direction::LEFT) {
                    targetNPC->SetDirection(Character::Direction::RIGHT);
                } 
                else if (playerDir == Character::Direction::RIGHT) {
                    targetNPC->SetDirection(Character::Direction::LEFT);
                }

                // Grab the list of dialogue lines from the NPC
                m_CurrentDialogueLines = targetNPC->Interact();
                m_CurrentDialogueIndex = 0; // Start at the first line

                // Display the first line!
                if (!m_CurrentDialogueLines.empty()) {
                    m_DialogueText->SetText(m_CurrentDialogueLines[m_CurrentDialogueIndex]);
                }
            }
            else {
                // Check for an item and catch the name it returns!
                std::string collectedItem = m_Map->CollectItemAt(checkX, checkY, m_Character);
                
                if (!collectedItem.empty()) {
                    m_Character.StopMoving();
                    m_IsInDialogue = true; // Trigger your dialogue UI!
                    
                    // ---> TURN ON THE UI BOXES <---
                    m_DialogueBoxUI->SetVisible(true);
                    m_DialogueUI->SetVisible(true);
                    
                    // ---> SET THE TEXT <---
                    m_CurrentDialogueLines = { "Obtained 1x " + collectedItem + "!" };
                    m_CurrentDialogueIndex = 0;
                    m_DialogueText->SetText(m_CurrentDialogueLines[m_CurrentDialogueIndex]);
                    std::string itemID = m_Map->GetCurrentLevelPath() + "_" + std::to_string(checkX) + "_" + std::to_string(checkY);
                    GameConfig::LootedItems.insert(itemID);
                }
            }
        }

        // 1b. Handle Normal Movement
        // (This only happens if we are NOT in dialogue)
        glm::vec2 movement = m_Character.Update(m_Map);
        m_Map->Move(-movement.x, -movement.y);

        // 1c. SAFE DOOR LOGIC
        if (m_Character.HasHitDoor()) {
            std::string currentMap = m_Map->GetCurrentLevelPath();
            int doorX = m_Character.GetGridX();
            int doorY = m_Character.GetGridY();

            // 1. Build the unique key for the door we just stepped on
            std::string doorKey = currentMap + "_" + std::to_string(doorX) + "_" + std::to_string(doorY);

            // 2. Look up the key in our GameConfig dictionary
            auto it = GameConfig::DoorRouting.find(doorKey);
            
            if (it != GameConfig::DoorRouting.end()) {
                // WE FOUND THE DOOR! Teleport the player!
                GameConfig::WarpDestination dest = it->second;

                LOG_TRACE("Taking door to: {}", dest.levelPath);
                m_Map->LoadLevel(dest.levelPath);
                m_Character.SetGridPosition(dest.spawnX, dest.spawnY);
                m_Map->WarpTo(dest.spawnX, dest.spawnY);
                m_Character.StopMoving();
                m_Character.ClearDoorFlag(); 
            } else {
                // THE DOOR IS NOT IN THE DICTIONARY! 
                // Don't teleport, just log an error so we can fix it!
                LOG_ERROR("Unregistered door at: {}", doorKey);
                m_Character.ClearDoorFlag(); 
            }
        }
    }

    // ==========================================
    // 2. GENERAL UPDATES (Always runs)
    // ==========================================
    m_Map->Update(); 

    // ==========================================
    // 3. DRAWING (Always runs)
    // ==========================================
    m_Map->Draw();
    m_Character.Draw();
    
    if (m_IsInDialogue) {
        if (m_DialogueBoxUI) m_DialogueBoxUI->Draw();
        if (m_DialogueUI) m_DialogueUI->Draw();
    }
}
void App::End() {
    LOG_TRACE("End");
}
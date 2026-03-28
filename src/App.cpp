#include "App.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Character.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Text.hpp"
#include <iostream>

void App::Start() {
    LOG_TRACE("Start");
    
    m_Map = std::make_shared<Map>(); 
    
    // ==========================================
    // BUILD THE DIALOGUE UI
    // ==========================================
    m_DialogueBoxUI = std::make_shared<Util::GameObject>();
    auto boxImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Fonts/BWTextBox2.png"); // Your new image
    m_DialogueBoxUI->SetDrawable(boxImage);
    m_DialogueBoxUI->SetZIndex(9.0f); // Under the text, over the game
    m_DialogueBoxUI->SetVisible(false);

    // Move it to the bottom of the screen! 
    // (You may need to tweak these numbers depending on your window size)
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

    // ==========================================
    // 1. STATE MACHINE (Input & Logic)
    // ==========================================
    if (m_IsInDialogue) {
        // Only process the Z key press once per tap
        if (Util::Input::IsKeyDown(Util::Keycode::Z)) {
            
            // Are there more lines left in the conversation?
            if (m_CurrentDialogueIndex < m_CurrentDialogueLines.size() - 1) {
                // Yes! Move to the next line
                m_CurrentDialogueIndex++;
                
                // SAFE TEXT UPDATE (Fixes the SetText Error)
                m_DialogueText->SetText(m_CurrentDialogueLines[m_CurrentDialogueIndex]);
            } 
            else {
                // No more lines! End the dialogue.
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
        }

        // 1b. Handle Normal Movement
        // (This only happens if we are NOT in dialogue)
        glm::vec2 movement = m_Character.Update(m_Map);
        m_Map->Move(-movement.x, -movement.y);

        // 1c. SAFE DOOR LOGIC
        if (m_Character.HasHitDoor()) {
            GameConfig::WarpDestination dest;
            if (!m_IsIndoors) {
                LOG_TRACE("Warping Inside!");
                dest = GameConfig::WARP_PC_INSIDE;
                m_IsIndoors = true; 
            } else {
                LOG_TRACE("Warping Outside!");
                dest = GameConfig::WARP_TOWN_OUTSIDE;
                m_IsIndoors = false;
            }

            m_Map->LoadLevel(dest.levelPath);
            m_Character.SetGridPosition(dest.spawnX, dest.spawnY);
            m_Map->WarpTo(dest.spawnX, dest.spawnY);
            m_Character.StopMoving();
            m_Character.ClearDoorFlag(); 
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
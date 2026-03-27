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
    auto boxImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Fonts/TextBox1.png"); // Your new image
    m_DialogueBoxUI->SetDrawable(boxImage);
    m_DialogueBoxUI->SetZIndex(9.0f); // Under the text, over the game
    m_DialogueBoxUI->SetVisible(false);

    // Move it to the bottom of the screen! 
    // (You may need to tweak these numbers depending on your window size)
    m_DialogueBoxUI->m_Transform.scale = {0.8f, 0.6f};          // Shrink the massive image
    m_DialogueBoxUI->m_Transform.translation = {0.0f, -280.0f};

    // 2. The Text
    m_DialogueUI = std::make_shared<Util::GameObject>();
    m_DialogueText = std::make_shared<Util::Text>(
        RESOURCE_DIR "/Fonts/sserife.fon", 
        72,                              
        "...",                           
        Util::Color(50, 50, 50) // Dark Grey
    );
    m_DialogueUI->SetDrawable(m_DialogueText);
    m_DialogueUI->SetZIndex(10.0f);  // On top of the box!
    m_DialogueUI->SetVisible(false);

    // Position the text slightly offset from the center of the box so it starts top-left
    m_DialogueUI->m_Transform.translation = {-130.0f, -260.0f};
    
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
        // --- DIALOGUE STATE ---
        // If we are talking, the ONLY thing the player can do is close the box.
        if (Util::Input::IsKeyDown(Util::Keycode::Z)) { 
            m_IsInDialogue = false;
            m_DialogueUI->SetVisible(false); // Hide the UI
            m_DialogueBoxUI->SetVisible(false); // Hide box
        }
    } 
    else {
        // --- NORMAL GAME STATE ---
        // 1. Change m_Player-> to m_Character.
        m_Character.HandleInput(m_Map);

        // 1a. Check for Interaction (Pressing 'Z' to talk)
        if (Util::Input::IsKeyDown(Util::Keycode::Z) && !m_Character.IsMoving()) {
            int checkX = m_Character.GetGridX();
            int checkY = m_Character.GetGridY();

            // 2. Add "Character::Direction::" before your directions
            Character::Direction facing = m_Character.GetFacingDirection();
            if (facing == Character::Direction::UP)    checkY -= 1;
            if (facing == Character::Direction::DOWN)  checkY += 1;
            if (facing == Character::Direction::LEFT)  checkX -= 1;
            if (facing == Character::Direction::RIGHT) checkX += 1;

            // Ask the map if an NPC is there
            auto targetNPC = m_Map->GetNPCAt(checkX, checkY);
            if (targetNPC) {
                std::string text = targetNPC->Interact();
                m_DialogueText->SetText(text); 
                m_DialogueUI->SetVisible(true);
                m_DialogueBoxUI->SetVisible(true);
                m_IsInDialogue = true;          
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
    // We still update the map so water animations keep playing during dialogue!
    m_Map->Update(); 

    // ==========================================
    // 3. DRAWING (Always runs)
    // ==========================================
    m_Map->Draw();
    m_Character.Draw();
    
    // Make sure your dialogue UI actually gets drawn!
    // If your m_DialogueUI is a Util::GameObject, you need to draw it last so it's on top.
    if (m_IsInDialogue) {
        if (m_DialogueBoxUI) m_DialogueBoxUI->Draw();
        m_DialogueUI->Draw(); 
        if (m_DialogueUI) m_DialogueUI->Draw();
    }
}

void App::End() {
    LOG_TRACE("End");
}
#include "App.hpp"
#include "Map.hpp"
#include "Character.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <iostream>

void App::Start() {
    LOG_TRACE("Start");
    
    m_Map = std::make_shared<Map>(); 
    
    m_CurrentState = State::UPDATE;
}

void App::Update() {
    m_Map->Update();
    
    // Handle quitting
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    glm::vec2 movement = m_Character.Update(m_Map);
    m_Map->Move(-movement.x, -movement.y);

    // --- SAFE DOOR LOGIC ---
// --- SAFE DOOR LOGIC ---
    if (m_Character.HasHitDoor()) {
        
        // 1. Determine the destination data
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

        // 2. Execute the warp using the unified data!
        m_Map->LoadLevel(dest.levelPath);
        m_Character.SetGridPosition(dest.spawnX, dest.spawnY);
        m_Map->WarpTo(dest.spawnX, dest.spawnY);
        m_Character.StopMoving();

        // 3. Clear the flag
        m_Character.ClearDoorFlag(); 
    }
    
    m_Map->Draw();
    m_Character.Draw();
}

void App::End() {
    LOG_TRACE("End");
}
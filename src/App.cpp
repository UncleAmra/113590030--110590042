#include "App.hpp"
#include "Map.hpp"
#include "Character.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "iostream"

void App::Start() {
    LOG_TRACE("Start");
    
    m_Map = std::make_shared<Map>(); 
    
    m_CurrentState = State::UPDATE;
}

void App::Update() {
    m_Map->Draw();
    m_Map->Update(0.016f);
    //m_Character.Update();
    
    // 4. Handle quitting
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    glm::vec2 movement = m_Character.Update(m_Map);
    m_Map->Move(-movement.x, -movement.y);

// --- SAFE DOOR LOGIC ---
    if (m_Character.HasHitDoor()) {
        
        // Are we outside trying to go IN?
        if (!m_IsIndoors) {
            LOG_TRACE("Warping Inside!");
            m_Map->LoadLevel(RESOURCE_DIR "/inside.csv");
            
            // 1. TELEPORT RED TO INDOOR SPAWN
            // Change these to the tile right above your exit mat in inside.csv
            int indoorSpawnX = 5; 
            int indoorSpawnY = 8; 
            m_Character.SetGridPosition(indoorSpawnX, indoorSpawnY);
            
            // 2. SHIFT CAMERA TO NEW COORDINATES
            float shiftX = -288.0f + (indoorSpawnX * 48.0f);
            float shiftY = 288.0f - (indoorSpawnY * 48.0f);
            m_Map->Move(-shiftX, -shiftY);
            
            m_IsIndoors = true; 
        } 
        // Or are we inside trying to go OUT?
        else {
            LOG_TRACE("Warping Outside!");
            m_Map->LoadLevel(RESOURCE_DIR "/level.csv");
            
            // 1. TELEPORT RED TO OUTDOOR SPAWN
            // Change these to the dirt tile right below your door in level.csv
            int outdoorSpawnX = 10;
            int outdoorSpawnY = 11;
            m_Character.SetGridPosition(outdoorSpawnX, outdoorSpawnY);
            
            // 2. SHIFT CAMERA TO NEW COORDINATES
            float shiftX = -288.0f + (outdoorSpawnX * 48.0f);
            float shiftY = 288.0f - (outdoorSpawnY * 48.0f);
            m_Map->Move(-shiftX, -shiftY);
            
            m_IsIndoors = false;
        }

        // FORCE THE LOOP TO STOP!
        m_Character.ClearDoorFlag(); 
    }
}

void App::End() {
    LOG_TRACE("End");
}
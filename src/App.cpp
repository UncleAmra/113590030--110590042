#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");
    
    m_Map = std::make_shared<Map>(); 
    
    m_CurrentState = State::UPDATE;
}

void App::Update() {
    m_Map->Draw();
    //m_Character.Update();
 
    glm::vec2 movement = m_Character.Update(m_Map);
    //glm::vec2 playerMovement = m_Character.Update();
    m_Map->Move(-movement.x, -movement.y);
    
    // 4. Handle quitting
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() {
    LOG_TRACE("End");
}
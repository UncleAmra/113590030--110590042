#include "StartMenu.hpp"
#include "ResourceManager.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

StartMenu::StartMenu(std::shared_ptr<Util::Renderer> renderer) {
    // 1. Box Setup (617x720)
    m_BoxUI = std::make_shared<Util::GameObject>();
    auto startBoxImg = ResourceManager::GetImageStore().Get(RESOURCE_DIR "/UI/MenuBoxUI2.png"); 
    m_BoxUI->SetDrawable(startBoxImg);
    m_BoxUI->SetZIndex(90.0f); 
    m_BoxUI->m_Transform.scale = {1.0f, 1.0f}; 
    m_BoxUI->m_Transform.translation = {331.5f, 0.0f}; 

    // 2. Text Setup
    m_TextUI = std::make_shared<Util::GameObject>();
    auto menuText = std::make_shared<Util::Text>(
        RESOURCE_DIR "/Fonts/micross.ttf", 32, 
        "BAG\nSAVE\nEXIT", 
        Util::Color(50, 50, 50)
    );
    m_TextUI->SetDrawable(menuText);
    m_TextUI->SetZIndex(91.0f); 
    m_TextUI->m_Transform.translation = {170.0f, 220.0f}; 

    // 3. Cursor Setup
    m_CursorUI = std::make_shared<Util::GameObject>();
    auto cursorImg = ResourceManager::GetImageStore().Get(RESOURCE_DIR "/UI/Cursor.png"); 
    m_CursorUI->SetDrawable(cursorImg);
    m_CursorUI->SetZIndex(92.0f); 

    // 4. Add to Renderer and hide by default
    renderer->AddChild(m_BoxUI);
    renderer->AddChild(m_TextUI);
    renderer->AddChild(m_CursorUI);
    
    SetVisible(false);
}

void StartMenu::SetVisible(bool visible) {
    m_BoxUI->SetVisible(visible);
    m_TextUI->SetVisible(visible);
    m_CursorUI->SetVisible(visible);
    
    if (visible) {
        m_CursorIndex = 0; // Always reset cursor to top when opened
        UpdateCursorPosition();
    }
}

StartMenu::MenuOption StartMenu::Update() {
    // Handle Navigation
    if (Util::Input::IsKeyDown(Util::Keycode::UP) || Util::Input::IsKeyDown(Util::Keycode::W)) {
        m_CursorIndex--;
        if (m_CursorIndex < 0) m_CursorIndex = MAX_OPTIONS - 1; 
        UpdateCursorPosition();
    }
    if (Util::Input::IsKeyDown(Util::Keycode::DOWN) || Util::Input::IsKeyDown(Util::Keycode::S)) {
        m_CursorIndex++;
        if (m_CursorIndex >= MAX_OPTIONS) m_CursorIndex = 0; 
        UpdateCursorPosition();
    }

    // Handle Selection
    if (Util::Input::IsKeyDown(Util::Keycode::RETURN) || Util::Input::IsKeyDown(Util::Keycode::Z)) {
        return static_cast<MenuOption>(m_CursorIndex);
    }

    return MenuOption::NONE; // Nothing selected this frame
}

void StartMenu::UpdateCursorPosition() {
    float cursorBaseY = 260.0f;   
    float cursorSpacing = 40.0f;  
    float cursorX = 100.0f;       
    m_CursorUI->m_Transform.translation = {cursorX, cursorBaseY - (m_CursorIndex * cursorSpacing)};
}
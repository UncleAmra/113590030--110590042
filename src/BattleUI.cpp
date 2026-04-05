#include "BattleUI.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Image.hpp"
#include "Util/Renderer.hpp"
#include "Util/Logger.hpp"
#include "ResourceManager.hpp"

const std::string BATTLE_RES = std::string(RESOURCE_DIR) + "/battle/";
const std::string FONT_PATH = std::string(RESOURCE_DIR) + "/font.ttf"; // <-- CHANGE THIS TO YOUR ACTUAL FONT FILE!

BattleUI::BattleUI(std::shared_ptr<Util::Renderer> renderer) : m_Renderer(renderer) {
    // ==========================================
    // MASTER SCALE: Adjust this if things are still too small/big!
    // ==========================================
    glm::vec2 battleScale = {4.0f, 4.0f}; 
    
    // ==========================================
    // 1. BACKGROUND & BASES
    // ==========================================
    m_Background = std::make_shared<Util::GameObject>();
    m_Background->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "battle_bg_grass.png"));
    // Scaled up to 6.0f to completely eliminate those black borders!
    m_Background->m_Transform.scale = {6.0f, 6.0f};
    m_Background->m_Transform.translation = {0.0f, 80.0f}; 
    m_Background->SetZIndex(10); 

    m_PlayerBase = std::make_shared<Util::GameObject>();
    m_PlayerBase->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "player_base_grass.png"));
    m_PlayerBase->m_Transform.scale = battleScale;
    m_PlayerBase->m_Transform.translation = {-250.0f, -80.0f}; 
    m_PlayerBase->SetZIndex(11); 

    m_EnemyBase = std::make_shared<Util::GameObject>();
    m_EnemyBase->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "enemy_base_grass.png"));
    m_EnemyBase->m_Transform.scale = battleScale;
    m_EnemyBase->m_Transform.translation = {250.0f, 150.0f}; 
    m_EnemyBase->SetZIndex(11);

    // ==========================================
    // 2. SPRITES
    // ==========================================
    m_EnemySprite = std::make_shared<Util::GameObject>();
    m_EnemySprite->m_Transform.scale = battleScale;
    m_EnemySprite->m_Transform.translation = {250.0f, 200.0f}; 
    m_EnemySprite->SetZIndex(12);

    m_PlayerSprite = std::make_shared<Util::GameObject>();
    m_PlayerSprite->m_Transform.scale = battleScale;
    // Shifted X to -270.0f so Charmander sits dead-center on the grass ellipse
    m_PlayerSprite->m_Transform.translation = {-270.0f, -30.0f}; 
    m_PlayerSprite->SetZIndex(12);

    // ==========================================
    // 3. PANELS (HP Bars)
    // ==========================================
    m_EnemyPanel = std::make_shared<Util::GameObject>();
    m_EnemyPanel->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "enemy_panel.png"));
    m_EnemyPanel->m_Transform.scale = battleScale;
    // Pushed further right and slightly down
    m_EnemyPanel->m_Transform.translation = {-420.0f, 220.0f}; 
    m_EnemyPanel->SetZIndex(13);

    m_PlayerPanel = std::make_shared<Util::GameObject>();
    m_PlayerPanel->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "player_panel.png"));
    m_PlayerPanel->m_Transform.scale = battleScale;
    // Tucked deeper into the bottom right corner
    m_PlayerPanel->m_Transform.translation = {400.0f, -100.0f}; 
    m_PlayerPanel->SetZIndex(13);

    // ==========================================
    // 4. DIALOGUE & COMMAND MENU
    // ==========================================
    m_DialogueBox = std::make_shared<Util::GameObject>();
    m_DialogueBox->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "dialogue_box.png"));
    // Scaled wider to close the gap between the two boxes
    m_DialogueBox->m_Transform.scale = {4.0f, 4.0f}; 
    // Shifted left to hug the window edge
    m_DialogueBox->m_Transform.translation = {-160.0f, -280.0f}; 
    m_DialogueBox->SetZIndex(14); 

    m_CommandBox = std::make_shared<Util::GameObject>();
    m_CommandBox->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "command_box.png"));
    m_CommandBox->m_Transform.scale = battleScale;
    // Shifted right to hug the window edge
    m_CommandBox->m_Transform.translation = {400.0f, -270.0f}; 
    m_CommandBox->SetZIndex(15); 

    m_MenuCursor = std::make_shared<Util::GameObject>();
    m_MenuCursor->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "cursor.png"));
    m_MenuCursor->m_Transform.scale = battleScale;
    // Re-aligned X to sit perfectly next to the shifted "FIGHT" text
    m_MenuCursor->m_Transform.translation = {200.0f, -240.0f}; 
    m_MenuCursor->SetZIndex(16); 

    // ADD ALL GAMEOBJECTS TO RENDERER
    m_Renderer->AddChild(m_Background);
    m_Renderer->AddChild(m_PlayerBase);
    m_Renderer->AddChild(m_EnemyBase);
    m_Renderer->AddChild(m_EnemySprite);
    m_Renderer->AddChild(m_PlayerSprite);
    m_Renderer->AddChild(m_EnemyPanel);
    m_Renderer->AddChild(m_PlayerPanel);
    m_Renderer->AddChild(m_DialogueBox);
    m_Renderer->AddChild(m_CommandBox);
    m_Renderer->AddChild(m_MenuCursor);
    
    Hide();
}

void BattleUI::Show(std::vector<std::shared_ptr<Pokemon>> playerParty, std::shared_ptr<Pokemon> wildPokemon) {
    m_UIState = UIState::ANIMATING;
    m_IsVisible = true;
    m_BattleLogic = std::make_unique<BattleManager>(playerParty[0], wildPokemon, true);
    
    // 1. Isolate the names and make ONLY the names lowercase
    std::string enemyName = wildPokemon->GetName();
    std::transform(enemyName.begin(), enemyName.end(), enemyName.begin(), ::tolower);
    
    std::string playerName = playerParty[0]->GetName();
    std::transform(playerName.begin(), playerName.end(), playerName.begin(), ::tolower);

    // 2. Load BOTH frames
    m_EnemyFrame1 = ResourceManager::GetImageStore().Get(BATTLE_RES + enemyName + "_front_1.png");
    m_EnemyFrame2 = ResourceManager::GetImageStore().Get(BATTLE_RES + enemyName + "_front_2.png");

    m_PlayerFrame1 = ResourceManager::GetImageStore().Get(BATTLE_RES + playerName + "_back_1.png");
    m_PlayerFrame2 = ResourceManager::GetImageStore().Get(BATTLE_RES + playerName + "_back_2.png");

    m_EnemySprite->SetDrawable(m_EnemyFrame1);
    m_PlayerSprite->SetDrawable(m_PlayerFrame1);

    // ==========================================
    // 3. SET STARTING POSITIONS FOR SLIDE
    // ==========================================
    // Move player 600 pixels to the left
    m_PlayerBase->m_Transform.translation.x = -850.0f; // Target is -250
    m_PlayerSprite->m_Transform.translation.x = -870.0f; // Target is -270

    // Move enemy 600 pixels to the right
    m_EnemyBase->m_Transform.translation.x = 850.0f; // Target is 250
    m_EnemySprite->m_Transform.translation.x = 850.0f; // Target is 250

    // Trigger the sliding state instead of the intro animation!
    m_SlideTimer = 0;
    m_IsSlidingIn = true;
    m_IsIntroAnimating = false;
    m_IntroAnimTimer = 0;

    // 4. Set Initial Visibility
    m_Background->SetVisible(true);
    m_PlayerBase->SetVisible(true);
    m_EnemyBase->SetVisible(true);
    m_EnemySprite->SetVisible(true);
    m_PlayerSprite->SetVisible(true);

    // KEEP UI HIDDEN UNTIL SLIDE IS DONE!
    m_EnemyPanel->SetVisible(false);
    m_PlayerPanel->SetVisible(false);
    m_DialogueBox->SetVisible(false);
    m_CommandBox->SetVisible(false);
    m_MenuCursor->SetVisible(false);

    LOG_TRACE("Wild {} appeared!", wildPokemon->GetName());
}

bool BattleUI::Update() {
    if (!m_IsVisible) return false;

    // ==========================================
    // STATE 1: ANIMATIONS PLAYING
    // ==========================================
    if (m_UIState == UIState::ANIMATING) {
        if (m_IsSlidingIn) {
            m_SlideTimer++;
            m_PlayerBase->m_Transform.translation.x += 15.0f;
            m_PlayerSprite->m_Transform.translation.x += 15.0f;
            m_EnemyBase->m_Transform.translation.x -= 15.0f;
            m_EnemySprite->m_Transform.translation.x -= 15.0f;

            if (m_SlideTimer >= 40) {
                m_IsSlidingIn = false;
                m_EnemyPanel->SetVisible(true);
                m_PlayerPanel->SetVisible(true);
                m_DialogueBox->SetVisible(true);
                m_CommandBox->SetVisible(true);
                m_MenuCursor->SetVisible(true);
                m_IsIntroAnimating = true; // Chain to bounce
            }
        }
        else if (m_IsIntroAnimating) {
            m_IntroAnimTimer++;
            if (m_IntroAnimTimer == 30) {
                m_EnemySprite->SetDrawable(m_EnemyFrame2);
                m_PlayerSprite->SetDrawable(m_PlayerFrame2);
            }
            else if (m_IntroAnimTimer == 60) {
                m_EnemySprite->SetDrawable(m_EnemyFrame1);
                m_PlayerSprite->SetDrawable(m_PlayerFrame1);
                m_IsIntroAnimating = false; 
                
                // ANIMATION FINISHED! Hand control to the player!
                m_UIState = UIState::MAIN_MENU;
                m_CursorIndex = 0;
                UpdateCursorPosition();
            }
        }
    }
// ==========================================
    // STATE 2: MAIN COMMAND MENU (FIGHT/BAG/PKMN/RUN)
    // ==========================================
    else if (m_UIState == UIState::MAIN_MENU) {
        
        // Navigation
        if (Util::Input::IsKeyDown(Util::Keycode::UP) || Util::Input::IsKeyDown(Util::Keycode::DOWN)) {
            m_CursorIndex = (m_CursorIndex + 2) % 4; 
            UpdateCursorPosition();
        }
        else if (Util::Input::IsKeyDown(Util::Keycode::LEFT) || Util::Input::IsKeyDown(Util::Keycode::RIGHT)) {
            if (m_CursorIndex % 2 == 0) m_CursorIndex++; else m_CursorIndex--; 
            UpdateCursorPosition();
        }

        // Selection
        if (Util::Input::IsKeyDown(Util::Keycode::Z)) {
            if (m_CursorIndex == 0) {
                LOG_TRACE("Selected FIGHT! Entering Move Menu.");
                m_UIState = UIState::MOVE_MENU;
                m_CursorIndex = 0; // Reset cursor to top-left move
                UpdateCursorPosition();
            }
            else if (m_CursorIndex == 1) {
                LOG_TRACE("Selected BAG! (Not implemented yet)");
            }
            else if (m_CursorIndex == 2) {
                LOG_TRACE("Selected POKEMON! (Not implemented yet)");
            }
            else if (m_CursorIndex == 3) {
                LOG_TRACE("Selected RUN! Escaping...");
                // (Temporary) Tell the App to escape next frame
            }
        }
    }
    // ==========================================
    // STATE 3: MOVE SELECTION MENU
    // ==========================================
    else if (m_UIState == UIState::MOVE_MENU) {
        
        // Move Navigation (Same 2x2 grid math as the main menu!)
        if (Util::Input::IsKeyDown(Util::Keycode::UP) || Util::Input::IsKeyDown(Util::Keycode::DOWN)) {
            m_CursorIndex = (m_CursorIndex + 2) % 4; 
            UpdateCursorPosition();
        }
        else if (Util::Input::IsKeyDown(Util::Keycode::LEFT) || Util::Input::IsKeyDown(Util::Keycode::RIGHT)) {
            if (m_CursorIndex % 2 == 0) m_CursorIndex++; else m_CursorIndex--; 
            UpdateCursorPosition();
        }

        // Select a Move
        if (Util::Input::IsKeyDown(Util::Keycode::Z)) {
            // For now, we will just print which slot we selected and bounce back to the main menu!
            LOG_TRACE("PLAYER USED MOVE SLOT {}!", m_CursorIndex);
            
            // TODO: Here is where we will call m_BattleLogic->ExecuteTurn(m_CursorIndex)
            
            // Temporary: bounce back to main menu after "attacking"
            m_UIState = UIState::MAIN_MENU;
            m_CursorIndex = 0;
            UpdateCursorPosition();
        }

        // Cancel (X Key) to go back to Main Menu
        if (Util::Input::IsKeyDown(Util::Keycode::X)) {
            LOG_TRACE("Cancelled Move Selection! Back to Main Menu.");
            m_UIState = UIState::MAIN_MENU;
            m_CursorIndex = 0;
            UpdateCursorPosition();
        }
    }

    return true; 
}

void BattleUI::Hide() {
    m_IsVisible = false;
    m_Background->SetVisible(false);
    m_PlayerBase->SetVisible(false);
    m_EnemyBase->SetVisible(false);
    m_EnemySprite->SetVisible(false);
    m_PlayerSprite->SetVisible(false);
    m_EnemyPanel->SetVisible(false);
    m_PlayerPanel->SetVisible(false);
    m_DialogueBox->SetVisible(false);
    m_CommandBox->SetVisible(false);
    m_MenuCursor->SetVisible(false);
}


void BattleUI::UpdateCursorPosition() {
    // 0 = Top Left (FIGHT),     1 = Top Right (BAG)
    // 2 = Bottom Left (POKEMON), 3 = Bottom Right (RUN)
    
    float startX = 200.0f; // Left column X
    float startY = -240.0f; // Top row Y
    
    float xOffset = (m_CursorIndex % 2 == 1) ? 120.0f : 0.0f; // Shift right for 1 and 3
    float yOffset = (m_CursorIndex >= 2) ? -40.0f : 0.0f;     // Shift down for 2 and 3
    
    m_MenuCursor->m_Transform.translation = {startX + xOffset, startY + yOffset};
}
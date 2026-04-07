#include "BattleUI.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Image.hpp"
#include "Util/Renderer.hpp"
#include "Util/Logger.hpp"
#include "ResourceManager.hpp"

const std::string BATTLE_RES = std::string(RESOURCE_DIR) + "/battle/";
const std::string FONT_PATH = std::string(RESOURCE_DIR) + "/Fonts/micross.ttf"; 

BattleUI::BattleUI(std::shared_ptr<Util::Renderer> renderer) : m_Renderer(renderer) {
    // ==========================================
    // MASTER SCALE
    // ==========================================
    glm::vec2 battleScale = {4.0f, 4.0f}; 
    
    // ==========================================
    // 1. BACKGROUND & BASES
    // ==========================================
    m_Background = std::make_shared<Util::GameObject>();
    m_Background->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "battle_bg_grass.png"));
    m_Background->m_Transform.scale = {5.0f, 5.0f};
    m_Background->m_Transform.translation = {0.0f, 0.0f}; 
    m_Background->SetZIndex(10); 

    m_PlayerBase = std::make_shared<Util::GameObject>();
    m_PlayerBase->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "player_base_grass.png"));
    m_PlayerBase->m_Transform.scale = battleScale;
    m_PlayerBase->m_Transform.translation = {-250.0f, -120.0f}; 
    m_PlayerBase->SetZIndex(11); 

    m_EnemyBase = std::make_shared<Util::GameObject>();
    m_EnemyBase->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "enemy_base_grass.png"));
    m_EnemyBase->m_Transform.scale = battleScale;
    m_EnemyBase->m_Transform.translation = {400.0f, 80.0f}; 
    m_EnemyBase->SetZIndex(11);

    // ==========================================
    // 2. SPRITES
    // ==========================================
    m_EnemySprite = std::make_shared<Util::GameObject>();
    m_EnemySprite->m_Transform.scale = battleScale;
    m_EnemySprite->m_Transform.translation = {400.0f, 130.0f}; 
    m_EnemySprite->SetZIndex(12);

    m_PlayerSprite = std::make_shared<Util::GameObject>();
    m_PlayerSprite->m_Transform.scale = battleScale;
    m_PlayerSprite->m_Transform.translation = {-270.0f, -30.0f}; 
    m_PlayerSprite->SetZIndex(12);

    // ==========================================
    // 3. PANELS (HP Bars)
    // ==========================================
    m_EnemyPanel = std::make_shared<Util::GameObject>();
    m_EnemyPanel->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "enemy_panel.png"));
    m_EnemyPanel->m_Transform.scale = battleScale;
    m_EnemyPanel->m_Transform.translation = {-400.0f, 220.0f}; 
    m_EnemyPanel->SetZIndex(13);

    m_PlayerPanel = std::make_shared<Util::GameObject>();
    m_PlayerPanel->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "player_panel.png"));
    m_PlayerPanel->m_Transform.scale = battleScale;
    m_PlayerPanel->m_Transform.translation = {400.0f, -50.0f}; 
    m_PlayerPanel->SetZIndex(13);

    // ==========================================
    // 4. DIALOGUE & COMMAND MENU
    // ==========================================
    m_DialogueBox = std::make_shared<Util::GameObject>();
    m_DialogueBox->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "dialogue_box.png"));
    m_DialogueBox->m_Transform.scale = {4.0f, 4.0f}; 
    m_DialogueBox->m_Transform.translation = {-160.0f, -280.0f}; 
    m_DialogueBox->SetZIndex(14); 

    m_CommandBox = std::make_shared<Util::GameObject>();
    m_CommandBox->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "command_box.png"));
    m_CommandBox->m_Transform.scale = battleScale;
    m_CommandBox->m_Transform.translation = {400.0f, -270.0f}; 
    m_CommandBox->SetZIndex(15); 

    m_MoveBox = std::make_shared<Util::GameObject>();
    m_MoveBox->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "move_box.png")); 
    m_MoveBox->m_Transform.scale = {4.0f, 4.0f}; 
    m_MoveBox->m_Transform.translation = {0.0f, -250.0f}; 
    m_MoveBox->SetZIndex(14); 

    m_MenuCursor = std::make_shared<Util::GameObject>();
    m_MenuCursor->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "cursor.png"));
    m_MenuCursor->m_Transform.scale = battleScale;
    m_MenuCursor->m_Transform.translation = {200.0f, -240.0f}; 
    m_MenuCursor->SetZIndex(16); 

    m_DialogueText = std::make_shared<Util::Text>(FONT_PATH, 30, "What will you do?", Util::Color{0, 0, 0, 255});
    
    m_DialogueTextObj = std::make_shared<Util::GameObject>();
    m_DialogueTextObj->SetDrawable(m_DialogueText);
    m_DialogueTextObj->SetZIndex(100); 
    m_DialogueTextObj->m_Transform.translation = {-150.0f, -220.0f}; 
    
    // ADD MAIN GAMEOBJECTS TO RENDERER
    m_Renderer->AddChild(m_MoveBox);
    m_Renderer->AddChild(m_DialogueTextObj);
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

    // ==========================================
    // 5. MOVE TEXTS (MOVED HERE TO THE CONSTRUCTOR!)
    // ==========================================
    for (int i = 0; i < 4; i++) {
        m_MoveTexts[i] = std::make_shared<Util::Text>(FONT_PATH, 30, "-", Util::Color{0, 0, 0, 255});
        
        m_MoveTextObjs[i] = std::make_shared<Util::GameObject>();
        m_MoveTextObjs[i]->SetDrawable(m_MoveTexts[i]);
        m_MoveTextObjs[i]->SetZIndex(100); 
        
        float startX = -210.0f; 
        float startY = -240.0f; 
        float xOffset = (i % 2 == 1) ? 300.0f : 0.0f; 
        float yOffset = (i >= 2) ? -65.0f : 0.0f; 
        
        m_MoveTextObjs[i]->m_Transform.translation = {startX + xOffset, startY + yOffset};
        m_Renderer->AddChild(m_MoveTextObjs[i]);
    }
    
    Hide();
}

void BattleUI::Show(std::vector<std::shared_ptr<Pokemon>> playerParty, std::shared_ptr<Pokemon> wildPokemon) {
    m_BattleOver = false;
    m_UIState = UIState::ANIMATING;
    m_IsVisible = true;
    m_BattleLogic = std::make_unique<BattleManager>(playerParty[0], wildPokemon, true);
    m_MoveBox->SetVisible(false);
    
    m_DialogueText->SetText("What will you do?");
    
    std::string enemyName = wildPokemon->GetName();
    std::transform(enemyName.begin(), enemyName.end(), enemyName.begin(), ::tolower);
    
    std::string playerName = playerParty[0]->GetName();
    std::transform(playerName.begin(), playerName.end(), playerName.begin(), ::tolower);

    m_EnemyFrame1 = ResourceManager::GetImageStore().Get(BATTLE_RES + enemyName + "_front_1.png");
    m_EnemyFrame2 = ResourceManager::GetImageStore().Get(BATTLE_RES + enemyName + "_front_2.png");

    m_PlayerFrame1 = ResourceManager::GetImageStore().Get(BATTLE_RES + playerName + "_back_1.png");
    m_PlayerFrame2 = ResourceManager::GetImageStore().Get(BATTLE_RES + playerName + "_back_2.png");

    m_EnemySprite->SetDrawable(m_EnemyFrame1);
    m_PlayerSprite->SetDrawable(m_PlayerFrame1);

    //m_PlayerBase->m_Transform.translation.x = -850.0f;
    m_PlayerSprite->m_Transform.translation.x = -870.0f;

    m_EnemyBase->m_Transform.translation.x = 1000.0f;
    m_EnemySprite->m_Transform.translation.x = 1000.0f;

    m_SlideTimer = 0;
    m_IsSlidingIn = true;
    m_IsIntroAnimating = false;
    m_IntroAnimTimer = 0;

    m_Background->SetVisible(true);
    m_PlayerBase->SetVisible(true);
    m_EnemyBase->SetVisible(true);
    m_EnemySprite->SetVisible(true);
    m_PlayerSprite->SetVisible(true);

    m_EnemyPanel->SetVisible(false);
    m_PlayerPanel->SetVisible(false);
    m_DialogueBox->SetVisible(true);
    m_CommandBox->SetVisible(false);
    m_MenuCursor->SetVisible(false);
    m_DialogueTextObj->SetVisible(true); 

    LOG_TRACE("Wild {} appeared!", wildPokemon->GetName());
    
    // Load the Move Names!
    auto moves = playerParty[0]->GetMoves(); 
    
    for (size_t i = 0; i < 4; i++) {
        if (i < moves.size()) {
            m_MoveTexts[i]->SetText(moves[i]); 
        } else {
            m_MoveTexts[i]->SetText("-"); 
        }
    }
}

bool BattleUI::Update() {
    if (!m_IsVisible) return false;

    // ==========================================
    // STATE 1: ANIMATIONS PLAYING
    // ==========================================
    if (m_UIState == UIState::ANIMATING) {
        if (m_IsSlidingIn) {
            m_SlideTimer++;
            //m_PlayerBase->m_Transform.translation.x += 15.0f;
            m_PlayerSprite->m_Transform.translation.x += 15.0f;
            m_EnemyBase->m_Transform.translation.x -= 15.0f;
            m_EnemySprite->m_Transform.translation.x -= 15.0f;

            if (m_SlideTimer >= 40) {
                m_IsSlidingIn = false;
                m_EnemyPanel->SetVisible(true);
                m_PlayerPanel->SetVisible(true);
                m_DialogueTextObj->SetVisible(true); 
                m_IsIntroAnimating = true; 
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
                
                m_UIState = UIState::MAIN_MENU;
                m_CursorIndex = 0;
                UpdateCursorPosition();
            }
        }
    }
    // ==========================================
    // STATE 2: MAIN COMMAND MENU
    // ==========================================
    else if (m_UIState == UIState::MAIN_MENU) {
        
        if (Util::Input::IsKeyDown(Util::Keycode::UP) || Util::Input::IsKeyDown(Util::Keycode::DOWN)) {
            m_CursorIndex = (m_CursorIndex + 2) % 4; 
            UpdateCursorPosition();
        }
        else if (Util::Input::IsKeyDown(Util::Keycode::LEFT) || Util::Input::IsKeyDown(Util::Keycode::RIGHT)) {
            if (m_CursorIndex % 2 == 0) m_CursorIndex++; else m_CursorIndex--; 
            UpdateCursorPosition();
        }
        

        if (Util::Input::IsKeyDown(Util::Keycode::Z)) {
            if (m_CursorIndex == 0) {
                m_UIState = UIState::MOVE_MENU;
                m_CursorIndex = 0; 
                UpdateCursorPosition();
            }
            else if (m_CursorIndex == 3) {
                BattleManager::TurnResult result = m_BattleLogic->SelectAction(
                    BattleManager::Action::RUN);
                m_DialogueText->SetText(result.message);
                m_UIState = UIState::WAITING_TEXT;
                m_TextWaitTimer = 30;
                m_LastResult = result;
            }
        }
    }
    // ==========================================
    // STATE 3: MOVE SELECTION MENU
    // ==========================================
    else if (m_UIState == UIState::MOVE_MENU) {
        
        if (Util::Input::IsKeyDown(Util::Keycode::UP) || Util::Input::IsKeyDown(Util::Keycode::DOWN)) {
            m_CursorIndex = (m_CursorIndex + 2) % 4; 
            UpdateCursorPosition();
        }
        else if (Util::Input::IsKeyDown(Util::Keycode::LEFT) || Util::Input::IsKeyDown(Util::Keycode::RIGHT)) {
            if (m_CursorIndex % 2 == 0) m_CursorIndex++; else m_CursorIndex--; 
            UpdateCursorPosition();
        }

        if (Util::Input::IsKeyDown(Util::Keycode::Z)) {
            BattleManager::TurnResult result = m_BattleLogic->SelectMove(m_CursorIndex);
            
            m_DialogueText->SetText(result.message);
            
            m_UIState = UIState::WAITING_TEXT; 
            m_TextWaitTimer = 30; 
        }

        if (Util::Input::IsKeyDown(Util::Keycode::X)) {
            m_UIState = UIState::MAIN_MENU;
            m_CursorIndex = 0;
            UpdateCursorPosition();
        }
    }
    // ==========================================
    // STATE 4: WAITING FOR PLAYER TO READ TEXT
    // ==========================================
    else if (m_UIState == UIState::WAITING_TEXT) {
        if (m_TextWaitTimer > 0) {
            m_TextWaitTimer--;
        }
        else if (Util::Input::IsKeyDown(Util::Keycode::Z)) {
        // Check if battle is actually over
        auto state = m_BattleLogic->GetState();
        if (state == BattleManager::BattleState::BATTLE_WON ||
            state == BattleManager::BattleState::BATTLE_LOST) {
            m_BattleOver = true;
        } 
        else {
            m_DialogueText->SetText("What will you do?");
            m_UIState = UIState::MAIN_MENU;
            m_CursorIndex = 0;
            UpdateCursorPosition();
            }
        }
    }

    // ==========================================
    // UPDATE GRAPHICS BEFORE RETAINING CONTROL
    // ==========================================
    UpdateMenuVisibility();

    return true; 
}

void BattleUI::Hide() {
    m_IsVisible = false;
    m_MoveBox->SetVisible(false);
    m_DialogueTextObj->SetVisible(false);
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
    
    // Safely hide the text objects if they exist
    for (int i = 0; i < 4; i++) {
        if (m_MoveTextObjs[i]) {
            m_MoveTextObjs[i]->SetVisible(false);
        }
    }
}

void BattleUI::UpdateCursorPosition() {
    float startX = 0.0f; 
    float startY = 0.0f; 
    float xOffset = 0.0f; 
    float yOffset = 0.0f; 

    if (m_UIState == UIState::MAIN_MENU) {
        startX = 200.0f; 
        startY = -240.0f; 
        xOffset = (m_CursorIndex % 2 == 1) ? 220.0f : 0.0f; 
        yOffset = (m_CursorIndex >= 2) ? -65.0f : 0.0f; 
    }
    else if (m_UIState == UIState::MOVE_MENU) {
        startX = -250.0f; 
        startY = -240.0f; 
        xOffset = (m_CursorIndex % 2 == 1) ? 300.0f : 0.0f; 
        yOffset = (m_CursorIndex >= 2) ? -65.0f : 0.0f; 
    }
    
    m_MenuCursor->m_Transform.translation = {startX + xOffset, startY + yOffset};
}

void BattleUI::UpdateMenuVisibility() {
    bool isMainMenu = (m_UIState == UIState::MAIN_MENU);
    bool isMoveMenu = (m_UIState == UIState::MOVE_MENU);
    bool isWaitingText = (m_UIState == UIState::WAITING_TEXT);
    
    m_CommandBox->SetVisible(isMainMenu);
    m_MoveBox->SetVisible(isMoveMenu);

    for (int i = 0; i < 4; i++) {
        // Extra safeguard to make sure they aren't null
        if (m_MoveTextObjs[i]) {
            m_MoveTextObjs[i]->SetVisible(isMoveMenu);
        }
    }
    
    m_DialogueBox->SetVisible(isMainMenu || isWaitingText);
    m_DialogueTextObj->SetVisible(isMainMenu || isWaitingText);
    m_MenuCursor->SetVisible(isMainMenu || isMoveMenu);
}
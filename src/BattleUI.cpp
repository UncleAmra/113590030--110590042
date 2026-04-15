#include "BattleUI.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Image.hpp"
#include "Util/Renderer.hpp"
#include "Util/Logger.hpp"
#include "string"
#include <cmath>   // For std::sin
#include <cstdlib> // For rand()
#include <algorithm> // For std::clamp

const std::string POKEMON_RES = std::string(RESOURCE_DIR) + "/Pokemon/";
const std::string BATTLE_RES = std::string(RESOURCE_DIR) + "/battle/";
const std::string FONT_PATH = std::string(RESOURCE_DIR) + "/Fonts/PokemonClassic.ttf"; 

BattleUI::BattleUI(std::shared_ptr<Util::Renderer> renderer) : m_Renderer(renderer) {
    // ==========================================
    // MASTER SCALE
    // ==========================================
    glm::vec2 battleScale = {3.5f, 3.5f}; 
    m_PokemonMenu = std::make_shared<PokemonMenu>(renderer);
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
    m_EnemySprite->m_Transform.translation = {400.0f, 150.0f}; 
    m_EnemySprite->SetZIndex(12);

    m_PlayerSprite = std::make_shared<Util::GameObject>();
    m_PlayerSprite->m_Transform.scale = battleScale;
    m_PlayerSprite->m_Transform.translation = {-270.0f, -50.0f}; 
    m_PlayerSprite->SetZIndex(12);

    // ==========================================
    // 3. PANELS (HP Bars)
    // ==========================================
    m_EnemyPanel = std::make_shared<Util::GameObject>();
    m_EnemyPanel->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "enemy_panel.png"));
    m_EnemyPanel->m_Transform.scale = {5.0f, 4.0f};
    m_EnemyPanel->m_Transform.translation = {-340.0f, 280.0f}; 
    m_EnemyPanel->SetZIndex(13);

    m_PlayerPanel = std::make_shared<Util::GameObject>();
    m_PlayerPanel->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "player_panel.png"));
    m_PlayerPanel->m_Transform.scale = {4.8f, 4.0f};
    m_PlayerPanel->m_Transform.translation = {340.0f, -50.0f}; 
    m_PlayerPanel->SetZIndex(13);

    // --- NEW: DYNAMIC BARS ---
    m_EnemyHPBar = std::make_shared<Util::GameObject>();
    m_EnemyHPBar->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "bar_green.png"));
    m_EnemyHPBar->m_Transform.scale = {6.0f, 4.0f}; // Fixed scale Y
    m_EnemyHPBar->SetZIndex(14); 

    m_PlayerHPBar = std::make_shared<Util::GameObject>();
    m_PlayerHPBar->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "bar_green.png"));
    m_PlayerHPBar->m_Transform.scale = {4.8f, 4.0f}; // Fixed scale Y
    m_PlayerHPBar->SetZIndex(14); 

    m_PlayerEXPBar = std::make_shared<Util::GameObject>();
    m_PlayerEXPBar->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "bar_blue.png"));
    m_PlayerEXPBar->m_Transform.scale = {4.8f, 4.0f}; // Fixed scale Y
    m_PlayerEXPBar->SetZIndex(14); 

    // --- NEW: PLAYER HP NUMBERS ---
    m_PlayerHPText = std::make_shared<Util::Text>(FONT_PATH, 25, "HP: 50/50", Util::Color{0, 0, 0, 255});
    m_PlayerHPTextObj = std::make_shared<Util::GameObject>();
    m_PlayerHPTextObj->SetDrawable(m_PlayerHPText);
    m_PlayerHPTextObj->SetZIndex(14); 

    // ==========================================
    // 4. DIALOGUE & COMMAND MENU
    // ==========================================
    m_DialogueBox = std::make_shared<Util::GameObject>();
    m_DialogueBox->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "dialogue_box.png"));
    m_DialogueBox->m_Transform.scale = {5.33f, 4.0f}; 
    m_DialogueBox->m_Transform.translation = {0.20f, -265.0f}; 
    m_DialogueBox->SetZIndex(14); 

    m_CommandBox = std::make_shared<Util::GameObject>();
    m_CommandBox->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "command_box.png"));
    m_CommandBox->m_Transform.scale = {4.0f, 4.0f}; 
    m_CommandBox->m_Transform.translation = {400.0f, -260.0f}; 
    m_CommandBox->SetZIndex(15); 

    m_MoveBox = std::make_shared<Util::GameObject>();
    m_MoveBox->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "move_box.png")); 
    m_MoveBox->m_Transform.scale = {4.0f, 4.0f}; 
    m_MoveBox->m_Transform.translation = {0.0f, -250.0f}; 
    m_MoveBox->SetZIndex(14); 

    m_MenuCursor = std::make_shared<Util::GameObject>();
    m_MenuCursor->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "cursor.png"));
    m_MenuCursor->m_Transform.scale = {3.5f, 3.5f}; ;
    m_MenuCursor->m_Transform.translation = {170.0f, -200.0f}; 
    m_MenuCursor->SetZIndex(16); 

    m_DialogueText = std::make_shared<Util::Text>(FONT_PATH, 20, "What will you do?", Util::Color{255, 255, 255, 255});
    
    m_DialogueTextObj = std::make_shared<Util::GameObject>();
    m_DialogueTextObj->SetDrawable(m_DialogueText);
    m_DialogueTextObj->SetZIndex(100); 
    m_DialogueTextObj->m_Transform.translation = {-460.0f, -120.0f}; 
    
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
    
    // ADD THE NEW BARS AND HP TEXT
    m_Renderer->AddChild(m_EnemyHPBar);
    m_Renderer->AddChild(m_PlayerHPBar);
    m_Renderer->AddChild(m_PlayerEXPBar);
    m_Renderer->AddChild(m_PlayerHPTextObj);

    m_Renderer->AddChild(m_DialogueBox);
    m_Renderer->AddChild(m_CommandBox);
    m_Renderer->AddChild(m_MenuCursor);

    // levels

    // --- ENEMY LEVEL TEXT ---
    //m_EnemyLevelText = std::make_shared<Util::GameObject>();
    m_EnemyLevelTextDrawable = std::make_shared<Util::Text>(
    FONT_PATH, 26, "Lv99", Util::Color::FromName(Util::Colors::BLACK));
    
    // 2. Assign it to the GameObject
    m_EnemyLevelText = std::make_shared<Util::GameObject>();
    m_EnemyLevelText->SetDrawable(m_EnemyLevelTextDrawable);
    
    m_EnemyLevelText->SetZIndex(55.0f);
    m_EnemyLevelText->m_Transform.translation = {-195.0f, 295.0f}; 
    m_Renderer->AddChild(m_EnemyLevelText);


    // --- PLAYER LEVEL TEXT ---
    //m_PlayerLevelText = std::make_shared<Util::GameObject>();    
    m_PlayerLevelTextDrawable = std::make_shared<Util::Text>(
    FONT_PATH, 26, "Lv99", Util::Color::FromName(Util::Colors::BLACK));
    
    // 2. Assign it to the GameObject
    m_PlayerLevelText = std::make_shared<Util::GameObject>();
    m_PlayerLevelText->SetDrawable(m_PlayerLevelTextDrawable);
    
    m_PlayerLevelText->SetZIndex(55.0f);
    m_PlayerLevelText->m_Transform.translation = {582.0f, -13.0f}; 
    m_Renderer->AddChild(m_PlayerLevelText);

    // ==========================================
    // 5. MOVE TEXTS
    // ==========================================
    for (int i = 0; i < 4; i++) {
        m_MoveTexts[i] = std::make_shared<Util::Text>(FONT_PATH, 20, "-", Util::Color{0, 0, 0, 255});
        
        m_MoveTextObjs[i] = std::make_shared<Util::GameObject>();
        m_MoveTextObjs[i]->SetDrawable(m_MoveTexts[i]);
        m_MoveTextObjs[i]->SetZIndex(100); 
        
        float startX = -310.0f; 
        float startY = -220.0f; 
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
    
    // --- CACHE POKEMON FOR UI UPDATES ---
    m_PlayerPokemon = playerParty[0];
    m_EnemyPokemon = wildPokemon;
    m_EscapeSuccessful = false;

    SetDialogue("What will you do?"); 
    
    std::string enemyName = wildPokemon->GetName();
    std::transform(enemyName.begin(), enemyName.end(), enemyName.begin(), ::tolower);
    
    std::string playerName = playerParty[0]->GetName();
    std::transform(playerName.begin(), playerName.end(), playerName.begin(), ::tolower);

    m_EnemyFrame1 = ResourceManager::GetImageStore().Get(POKEMON_RES + enemyName + "_front_1.png");
    m_EnemyFrame2 = ResourceManager::GetImageStore().Get(POKEMON_RES + enemyName + "_front_2.png");

    m_PlayerFrame1 = ResourceManager::GetImageStore().Get(POKEMON_RES + playerName + "_back_1.png");
    m_PlayerFrame2 = ResourceManager::GetImageStore().Get(POKEMON_RES + playerName + "_back_2.png");

    m_EnemySprite->SetDrawable(m_EnemyFrame1);
    m_PlayerSprite->SetDrawable(m_PlayerFrame1);

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
    
    // Hide the dynamic UI elements during the intro sliding
    m_EnemyHPBar->SetVisible(false);
    m_PlayerHPBar->SetVisible(false);
    m_PlayerEXPBar->SetVisible(false);
    m_PlayerHPTextObj->SetVisible(false);

    m_DialogueBox->SetVisible(true);
    m_CommandBox->SetVisible(false);
    m_MenuCursor->SetVisible(false);
    m_DialogueTextObj->SetVisible(true); 

    LOG_TRACE("Wild {} appeared!", wildPokemon->GetName());

        // Get the levels from the Pokemon objects
    int playerLevel = m_PlayerPokemon->GetLevel();
    int enemyLevel = m_EnemyPokemon->GetLevel();
    // ADD THESE TWO LINES:

    // Update the Util::Text components with the new strings
    m_PlayerLevelTextDrawable->SetText(std::to_string(playerLevel));
    m_EnemyLevelTextDrawable->SetText(std::to_string(enemyLevel));
    m_PlayerLevelText->SetVisible(false);
    m_EnemyLevelText->SetVisible(false);
    
    auto moves = playerParty[0]->GetMoves(); 
    for (size_t i = 0; i < 4; i++) {
        if (i < moves.size()) {
            m_MoveTexts[i]->SetText(moves[i]); 
        } else {
            m_MoveTexts[i]->SetText("-"); 
        }
    }
    m_DisplayPlayerHPPercent = (float)m_PlayerPokemon->GetCurrentHP() / m_PlayerPokemon->GetMaxHP();
    m_DisplayEnemyHPPercent = (float)m_EnemyPokemon->GetCurrentHP() / m_EnemyPokemon->GetMaxHP();
    
    if (m_PlayerPokemon->GetExpToNextLevel() > 0) {
        m_DisplayPlayerEXPPercent = (float)m_PlayerPokemon->GetCurrentExp() / m_PlayerPokemon->GetExpToNextLevel();
    } else {
        m_DisplayPlayerEXPPercent = 0.0f;
    }
}

bool BattleUI::Update() {
    if (!m_IsVisible) return false;

    // ==========================================
    // DYNAMIC UI UPDATES (SMOOTH BARS)
    // ==========================================
    // 1. Calculate the TRUE target percentages
    float targetPlayerHPPercent = 1.0f;
    float targetEnemyHPPercent = 1.0f;
    float targetEXPPercent = m_DisplayPlayerEXPPercent;

    if (m_PlayerPokemon && m_EnemyPokemon) {
        targetPlayerHPPercent = (float)m_PlayerPokemon->GetCurrentHP() / m_PlayerPokemon->GetMaxHP();
        targetEnemyHPPercent  = (float)m_EnemyPokemon->GetCurrentHP() / m_EnemyPokemon->GetMaxHP();
        
        if (m_AllowEXPAnimation && m_PlayerPokemon->GetExpToNextLevel() > 0) {
            targetEXPPercent = (float)m_PlayerPokemon->GetCurrentExp() / m_PlayerPokemon->GetExpToNextLevel();
        }
    }

    if (m_PlayerPokemon && m_EnemyPokemon && !m_IsSlidingIn) {
        float animSpeed = 0.01f; // Adjust this to make bars move faster/slower!

        // ANIMATE PLAYER HP
        if (m_DisplayPlayerHPPercent > targetPlayerHPPercent) m_DisplayPlayerHPPercent -= animSpeed;
        else if (m_DisplayPlayerHPPercent < targetPlayerHPPercent) m_DisplayPlayerHPPercent += animSpeed;
        if (std::abs(m_DisplayPlayerHPPercent - targetPlayerHPPercent) < animSpeed) m_DisplayPlayerHPPercent = targetPlayerHPPercent;

        // ANIMATE ENEMY HP
        if (m_DisplayEnemyHPPercent > targetEnemyHPPercent) m_DisplayEnemyHPPercent -= animSpeed;
        else if (m_DisplayEnemyHPPercent < targetEnemyHPPercent) m_DisplayEnemyHPPercent += animSpeed;
        if (std::abs(m_DisplayEnemyHPPercent - targetEnemyHPPercent) < animSpeed) m_DisplayEnemyHPPercent = targetEnemyHPPercent;

        // ANIMATE PLAYER EXP
        // If target is way lower than display, we leveled up! Snap to 0 so it visually rolls over.
        if (targetEXPPercent < m_DisplayPlayerEXPPercent && targetEXPPercent < 0.5f && m_DisplayPlayerEXPPercent > 0.5f) {
            m_DisplayPlayerEXPPercent = 0.0f; 
        }
        if (m_DisplayPlayerEXPPercent < targetEXPPercent) m_DisplayPlayerEXPPercent += animSpeed;
        if (std::abs(m_DisplayPlayerEXPPercent - targetEXPPercent) < animSpeed) m_DisplayPlayerEXPPercent = targetEXPPercent;

        // Ensure values stay within 0% to 100% bounds
        m_DisplayPlayerHPPercent = std::clamp(m_DisplayPlayerHPPercent, 0.0f, 1.0f);
        m_DisplayEnemyHPPercent = std::clamp(m_DisplayEnemyHPPercent, 0.0f, 1.0f);
        m_DisplayPlayerEXPPercent = std::clamp(m_DisplayPlayerEXPPercent, 0.0f, 1.0f);

        // 2. Text Update - Uses DISPLAY percent so numbers roll smoothly with the bar!
        int displayHP = std::round(m_DisplayPlayerHPPercent * m_PlayerPokemon->GetMaxHP());
        std::string hpStr = std::to_string(displayHP) + " / " + std::to_string(m_PlayerPokemon->GetMaxHP());
        m_PlayerHPText->SetText(hpStr);
        
        float hpTextLeftX = 350.0f; 
        float hpTextTopY = -70.0f;
        glm::vec2 hpTextSize = m_PlayerHPText->GetSize();
        m_PlayerHPTextObj->m_Transform.translation.x = hpTextLeftX + (hpTextSize.x / 2.0f);
        m_PlayerHPTextObj->m_Transform.translation.y = hpTextTopY - (hpTextSize.y / 2.0f);

        // 3. Update Visual Bars using DISPLAY percentages
        UpdateBar(m_EnemyHPBar,   m_DisplayEnemyHPPercent,   -405.0f, 248.0f, 5.1f, true);
        UpdateBar(m_PlayerHPBar,  m_DisplayPlayerHPPercent,  375.0f,  -60.0f, 5.0f, true);
        UpdateBar(m_PlayerEXPBar, m_DisplayPlayerEXPPercent, 145.0f,  -128.0f, 2.0f, false);
    }

    // ==========================================
    // STATE 1: ANIMATIONS PLAYING
    // ==========================================
    if (m_UIState == UIState::ANIMATING) {
        if (m_IsSlidingIn) {
            m_SlideTimer++;
            m_PlayerSprite->m_Transform.translation.x += 15.0f;
            m_EnemyBase->m_Transform.translation.x -= 15.0f;
            m_EnemySprite->m_Transform.translation.x -= 15.0f;

            if (m_SlideTimer >= 40) {
                m_IsSlidingIn = false;
                m_EnemyPanel->SetVisible(true);
                m_PlayerPanel->SetVisible(true);
                
                // Show the bars when the panels slide in
                m_EnemyHPBar->SetVisible(true);
                m_PlayerHPBar->SetVisible(true);
                m_PlayerEXPBar->SetVisible(true);
                m_PlayerHPTextObj->SetVisible(true);

                m_DialogueTextObj->SetVisible(true); 
                m_PlayerLevelText->SetVisible(true);
                m_EnemyLevelText->SetVisible(true);
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
            else if (m_CursorIndex == 1) {
                if (!m_Player) {
                    LOG_ERROR("CRASH AVERTED: m_Player is null! We forgot to link it in App.cpp.");
                    return false; // Stop before we crash!
                }
                if (!m_InventoryMenu) {
                    LOG_ERROR("CRASH AVERTED: m_InventoryMenu is null!");
                    return false;
                }
                // BAG
                // Switch to a new UI state to render the inventory list
                m_UIState = UIState::BAG_MENU; 
                // 1. Get raw inventory from player
                auto rawInv = m_Player->GetInventory(); 

                // 2. Build the categorized map
                std::map<ItemCategory, std::vector<std::pair<std::string, int>>> m_CategorizedItems;
                for (const auto& [name, data] : rawInv) {
                    if (data.quantity > 0) {
                        m_CategorizedItems[data.category].push_back({name, data.quantity});
                    }
                }
                m_InventoryMenu->Show(m_CategorizedItems);
                m_CursorIndex = 0; // Reset cursor for the new menu
                UpdateCursorPosition();
            }
            else if (m_CursorIndex == 2) {
                // POKEMON (Party Switch)
                m_UIState = UIState::POKEMON_MENU; 
                
                // Hide the main battle menu prompts
                //m_ActionText1->SetVisible(false);
                //m_ActionText2->SetVisible(false);
                //m_CursorUI->SetVisible(false);
                
                // Show the Pokemon Menu!
                m_PokemonMenu->Show(m_Player->GetParty());
            }
            else if (m_CursorIndex == 3) {
                bool isWildBattle = !m_IsTrainerBattle;
                
                // 1. Clear out any old text first
                while(!m_DialogueQueue.empty()) m_DialogueQueue.pop();

                if (!isWildBattle) {
                    // 2. Push to queue instead of SetDialogue
                    m_DialogueQueue.push("You can't run from a trainer battle!");
                } else {
                    int playerLevel = m_PlayerPokemon->GetLevel(); 
                    int enemyLevel = m_EnemyPokemon->GetLevel();
                    
                    int escapeChance = 50 + ((playerLevel - enemyLevel) * 5);
                    escapeChance = std::clamp(escapeChance, 10, 95); 

                    if (rand() % 100 < escapeChance) {
                        // 2. Push to queue instead of SetDialogue
                        m_DialogueQueue.push("Got away safely!");
                        m_EscapeSuccessful = true; 
                    } else {
                        // 2. Push to queue instead of SetDialogue
                        m_DialogueQueue.push("Failed to escape!");
                        m_EscapeSuccessful = false;
                        
                        // (Optional: If your BattleManager supports it, you could append 
                        // an enemy attack to the queue here so they punish the failed escape!)
                    }
                }
                
                m_UIState = UIState::WAITING_TEXT;
                m_TextWaitTimer = 15; // 3. Lowered to 15 to match our new snappy pacing!
                
                // 4. Start the message sequence!
                ProcessNextMessage();
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
            m_AllowEXPAnimation = false;
            // 1. Clear out any old text
            while(!m_DialogueQueue.empty()) m_DialogueQueue.pop();

            // 2. Split the giant text block by newlines (\n) and push them to our queue!
            std::stringstream ss(result.message);
            std::string line;
            while(std::getline(ss, line, '\n')) {
                if(!line.empty()) {
                    m_DialogueQueue.push(line);
                }
            }

            m_UIState = UIState::WAITING_TEXT; 
            m_TextWaitTimer = 15; // Short cooldown so they don't skip text accidentally
            
            // 3. Start the turn sequence by showing the FIRST message!
            ProcessNextMessage();
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
            
            if (m_EscapeSuccessful) {
                m_BattleOver = true;
                Hide(); 
                return true;
            }

            if (!m_DialogueQueue.empty()) {
                ProcessNextMessage();
                m_TextWaitTimer = 15; 
            }
            else {
                // --- NEW: CHECK IF BARS ARE STILL ANIMATING ---
                bool barsAnimating = (m_DisplayPlayerHPPercent != targetPlayerHPPercent) || 
                                     (m_DisplayEnemyHPPercent  != targetEnemyHPPercent)  || 
                                     (m_DisplayPlayerEXPPercent != targetEXPPercent);

                if (barsAnimating) {
                    // Do nothing! Force the player to wait for the bars to finish draining/filling!
                    // This is authentic Pokémon behavior!
                    return true;
                }

                // The queue is empty AND the bars are done moving. The full turn is officially over.
                auto state = m_BattleLogic->GetState();
                
                if (state == BattleManager::BattleState::BATTLE_WON) {
                    
                    // --- NEW: TRAINER TEAM LOGIC ---
                    if (m_IsTrainerBattle) {
                        m_CurrentEnemyIndex++;
                        
                        // Are there still Pokémon left in the trainer's team?
                        if (m_CurrentEnemyIndex < static_cast<int>(m_EnemyTeam.size())){
                            
                            // 1. Get the next Pokémon
                            m_EnemyPokemon = m_EnemyTeam[m_CurrentEnemyIndex];
                            
                            // 2. Load new sprites
                            std::string enemyName = m_EnemyPokemon->GetName();
                            std::transform(enemyName.begin(), enemyName.end(), enemyName.begin(), ::tolower);
                            m_EnemyFrame1 = ResourceManager::GetImageStore().Get(POKEMON_RES + enemyName + "_front_1.png");
                            m_EnemyFrame2 = ResourceManager::GetImageStore().Get(POKEMON_RES + enemyName + "_front_2.png");
                            m_EnemySprite->SetDrawable(m_EnemyFrame1);
                            
                            // 3. Reset the internal BattleManager for the new opponent
                            m_BattleLogic = std::make_unique<BattleManager>(m_PlayerPokemon, m_EnemyPokemon, true);
                            
                            // 4. Reset Enemy UI values
                            m_DisplayEnemyHPPercent = 1.0f;
                            m_EnemyLevelTextDrawable->SetText(std::to_string(m_EnemyPokemon->GetLevel()));
                            
                            // 5. Queue text and start the next turn!
                            m_DialogueQueue.push("Trainer sent out " + m_EnemyPokemon->GetName() + "!");
                            m_UIState = UIState::WAITING_TEXT;
                            m_TextWaitTimer = 15;
                            ProcessNextMessage();
                            
                            return true; // Keep the battle going!
                        }
                    }
                    
                    // If not trainer battle, or the whole team is defeated, you officially win!
                    m_BattleOver = true;
                    m_IsTrainerBattle = false; // Reset the flag
                    Hide(); 
                    return true;
                } 
                else if (state == BattleManager::BattleState::BATTLE_LOST ||
                         state == BattleManager::BattleState::BATTLE_ESCAPED) { 
                    
                    m_BattleOver = true;
                    m_IsTrainerBattle = false; // Reset the flag
                    Hide(); 
                    return true;
                }
                else {
                    SetDialogue("What will you do?"); 
                    m_UIState = UIState::MAIN_MENU;
                    m_CursorIndex = 0;
                    UpdateCursorPosition();
                }
            }
        }
    }
    // ==========================================
    // STATE 5: BAG MENU (IN-BATTLE)
    // ==========================================
    else if (m_UIState == UIState::BAG_MENU) {
        
        // 1. Let your InventoryMenu handle all the UP/DOWN/LEFT/RIGHT scrolling!
        // If it returns true, it means the user pressed 'X' or 'Escape' to back out.
        if (m_InventoryMenu->Update()) {
            m_InventoryMenu->Hide();
            m_UIState = UIState::MAIN_MENU;
            m_CursorIndex = 1; // Put cursor back on 'Bag'
            UpdateCursorPosition();
            return true;
        }

        // 2. Handle Item Confirmation (Z)
        if (Util::Input::IsKeyDown(Util::Keycode::Z)) {
            std::string selectedItem = m_InventoryMenu->GetSelectedItem();
            ItemCategory currentTab = m_InventoryMenu->GetCurrentTab();
            
            // If the slot is empty, do nothing
            if (selectedItem.empty()) return true; 

            // Prevent them from using Key Items or general items in a wild battle catch phase
            if (currentTab != ItemCategory::POKEBALLS) {
                // (Optional: You could play an error 'boop' sound here!)
                return true; 
            }

            // --- WE HAVE A VALID POKEBALL! ---
            m_InventoryMenu->Hide();
            
            // Clear old text 
            while(!m_DialogueQueue.empty()) m_DialogueQueue.pop();

            // 1. FIRE THE BACKEND LOGIC!
            // This single line deducts the inventory, rolls the catch, 
            // and adds the Pokemon to your party if successful!
            m_BattleLogic->UseItem(m_Player, selectedItem); 
            
            // Queue the throw text
            m_DialogueQueue.push("You threw a " + selectedItem + "!");
            
            // 2. READ THE RESULT FROM THE MANAGER
            if (m_BattleLogic->GetState() == BattleManager::BattleState::BATTLE_WON) {
                // Catch succeeded! 
                m_DialogueQueue.push("Gotcha! " + m_EnemyPokemon->GetName() + " was caught!");
                
                // IMPORTANT: Do NOT set m_BattleOver = true here!
                // Let your WAITING_TEXT state handle closing the battle 
                // so the player actually gets to read the "Gotcha!" text.
                
            } else {
                // Catch failed! Manager changed state to EXECUTING_ENEMY_TURN
                m_DialogueQueue.push("Oh no! The wild Pokemon broke free!");
                
                // Since the item failed, it's the enemy's turn to attack
                BattleManager::TurnResult enemyResult = m_BattleLogic->ExecuteEnemyMove(); 
                
                std::stringstream ss(enemyResult.message);
                std::string line;
                while(std::getline(ss, line, '\n')) {
                    if(!line.empty()) m_DialogueQueue.push(line);
                }
            }
            
            // 3. Trigger the text reading state
            m_UIState = UIState::WAITING_TEXT;
            m_TextWaitTimer = 15;
            ProcessNextMessage();
        }
    }

    else if (m_UIState == UIState::POKEMON_MENU) {
        
        // 1. Let the menu handle UP/DOWN scrolling
        // If it returns true, the player pressed 'X' or 'Escape' to back out
        if (m_PokemonMenu->Update()) {
            m_PokemonMenu->Hide();
            m_UIState = UIState::MAIN_MENU;
            m_CursorIndex = 2; // Put cursor back on 'Pokemon'
            UpdateCursorPosition();
            SetDialogue("What will you do?"); // Restore standard battle text
            return true;
        }

        // 2. Handle choosing a Pokemon to switch in (Z key)
        if (Util::Input::IsKeyDown(Util::Keycode::Z)) {
            int selectedIdx = m_PokemonMenu->GetSelectedIndex();
            auto party = m_Player->GetParty();
            
            if (selectedIdx >= 0 && selectedIdx < static_cast<int>(party.size())) {
                auto selectedPokemon = party[selectedIdx];
                
                // --- SAFETY CHECKS ---
                if (selectedPokemon->GetCurrentHP() <= 0) {
                    LOG_INFO("Cannot switch to fainted Pokemon!");
                    return true;
                }
                if (selectedPokemon == m_PlayerPokemon) {
                    LOG_INFO("Pokemon is already in battle!");
                    return true; 
                }

                // --- VALID SWITCH! ---
                m_PokemonMenu->Hide();
                
                // Clear old text and queue the switch messages
                while(!m_DialogueQueue.empty()) m_DialogueQueue.pop();
                m_DialogueQueue.push("Come back, " + m_PlayerPokemon->GetName() + "!");
                m_DialogueQueue.push("Go! " + selectedPokemon->GetName() + "!");
                
                // 1. Update the UI's active Pokemon
                m_PlayerPokemon = selectedPokemon;
                
                // 2. Tell your backend BattleManager about the new Pokemon!
                // (Make sure you have this setter in BattleManager.hpp)
                m_BattleLogic->SetPlayerPokemon(m_PlayerPokemon); 
                
                // 3. Load the new back sprites based on the new Pokemon's name
                std::string pName = m_PlayerPokemon->GetName();
                // Ensure name is lowercase to match your file paths if needed:
                std::transform(pName.begin(), pName.end(), pName.begin(), ::tolower);
                
                m_PlayerFrame1 = ResourceManager::GetImageStore().Get(RESOURCE_DIR "/Pokemon/" + pName + "_back_1.png");
                m_PlayerFrame2 = ResourceManager::GetImageStore().Get(RESOURCE_DIR "/Pokemon/" + pName + "_back_2.png");
                m_PlayerSprite->SetDrawable(m_PlayerFrame1);
                
                // 4. Update the visual HP bar and Level text
                m_DisplayPlayerHPPercent = (float)m_PlayerPokemon->GetCurrentHP() / m_PlayerPokemon->GetMaxHP();
                m_PlayerLevelTextDrawable->SetText(std::to_string(m_PlayerPokemon->GetLevel()));
                
                // 5. Switching takes your turn! Execute the enemy's attack.
                BattleManager::TurnResult enemyResult = m_BattleLogic->ExecuteEnemyMove(); 
                std::stringstream ss(enemyResult.message);
                std::string line;
                while(std::getline(ss, line, '\n')) {
                    if(!line.empty()) m_DialogueQueue.push(line);
                }
                
                // 6. Trigger the text reading state
                m_UIState = UIState::WAITING_TEXT;
                m_TextWaitTimer = 15;
                ProcessNextMessage();
            }
        }
    }
    
    // Procedural Animations
    if (!m_IsSlidingIn && !m_IsIntroAnimating) {
        m_AnimTime++;
        
        float bobOffset1 = std::sin(m_AnimTime * 0.03f) * 1.5f; 
        float bobOffset2 = std::sin(m_AnimTime * 0.04f) * 2.5f; 

        m_EnemySprite->m_Transform.translation.y = 150.0f + bobOffset1;
        m_PlayerSprite->m_Transform.translation.y = -30.0f + bobOffset2;
        
        m_EnemySprite->m_Transform.scale.y = 4.0f;
        m_PlayerSprite->m_Transform.scale.y = 4.0f;

        // --- PLAYER ANIMATIONS ---
        if (m_PlayerLungeTimer > 0) {
            m_PlayerLungeTimer--;
            m_PlayerSprite->m_Transform.translation.x = -270.0f + 40.0f; // Player Lunges
        } else if (m_PlayerShakeTimer > 0 && m_EnemyLungeTimer < 5) {
            m_PlayerShakeTimer--;
            float shake = (rand() % 17) - 8; 
            m_PlayerSprite->m_Transform.translation.x = -270.0f + shake; // Player takes Damage
        } else {
            m_PlayerSprite->m_Transform.translation.x = -270.0f; // Idle
        }

        // --- ENEMY ANIMATIONS ---
        if (m_EnemyLungeTimer > 0) {
            m_EnemyLungeTimer--;
            m_EnemySprite->m_Transform.translation.x = 400.0f - 40.0f;   // Enemy Lunges
        } else if (m_EnemyShakeTimer > 0 && m_PlayerLungeTimer < 5) {
            m_EnemyShakeTimer--;
            float shake = (rand() % 17) - 8; 
            m_EnemySprite->m_Transform.translation.x = 400.0f + shake;   // Enemy takes Damage
        } else {
            m_EnemySprite->m_Transform.translation.x = 400.0f; // Idle
        }
    }

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
    if (m_EnemyHPBar) m_EnemyHPBar->SetVisible(false);
    if (m_PlayerHPBar) m_PlayerHPBar->SetVisible(false);
    if (m_PlayerEXPBar) m_PlayerEXPBar->SetVisible(false);
    if (m_PlayerHPTextObj) m_PlayerHPTextObj->SetVisible(false);
    
    // ADD THESE TWO LINES:
  
    
    // --- HIDE THE NEW UI ---
    if (m_EnemyHPBar) m_EnemyHPBar->SetVisible(false);
    if (m_PlayerHPBar) m_PlayerHPBar->SetVisible(false);
    if (m_PlayerEXPBar) m_PlayerEXPBar->SetVisible(false);
    if (m_PlayerHPTextObj) m_PlayerHPTextObj->SetVisible(false);
    if (m_PlayerLevelText) m_PlayerLevelText->SetVisible(false);
    if (m_EnemyLevelText) m_EnemyLevelText->SetVisible(false);
    m_DialogueBox->SetVisible(false); 
    m_CommandBox->SetVisible(false);
    m_MenuCursor->SetVisible(false);
    
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
        startY = -230.0f; 
        xOffset = (m_CursorIndex % 2 == 1) ? 220.0f : 0.0f; 
        yOffset = (m_CursorIndex >= 2) ? -65.0f : 0.0f; 
    }
    else if (m_UIState == UIState::MOVE_MENU) {
        startX = -420.0f; 
        startY = -220.0f; 
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
        if (m_MoveTextObjs[i]) {
            m_MoveTextObjs[i]->SetVisible(isMoveMenu);
        }
    }
    
    m_DialogueBox->SetVisible(isMainMenu || isWaitingText);
    m_DialogueTextObj->SetVisible(isMainMenu || isWaitingText);
    m_MenuCursor->SetVisible(isMainMenu || isMoveMenu);
}

void BattleUI::SetDialogue(const std::string& text) {
    m_DialogueText->SetText(text);

    glm::vec2 textSize = m_DialogueText->GetSize(); 
    
    float fixedLeftX = -580.0f; 
    float fixedTopY = -200.0f;  

    m_DialogueTextObj->m_Transform.translation.x = fixedLeftX + (textSize.x / 2.0f);
    m_DialogueTextObj->m_Transform.translation.y = fixedTopY - (textSize.y / 2.0f);
}

// =======================================================
// NEW HELPER: Keeps the left edge fixed while scaling!
// =======================================================
void BattleUI::UpdateBar(std::shared_ptr<Util::GameObject> bar, float percent, float leftEdgeX, float fixedY, float maxScale, bool isHPBar) {
    percent = std::clamp(percent, 0.0f, 1.0f);
    
    if (percent <= 0.0f) {
        bar->SetVisible(false);
        return; 
    } else {
        bar->SetVisible(true);
    }

    // --- COLOR LOGIC ---
    if (isHPBar) {
        if (percent > 0.50f) bar->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "bar_green.png"));
        else if (percent > 0.20f) bar->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "bar_yellow.png"));
        else bar->SetDrawable(ResourceManager::GetImageStore().Get(BATTLE_RES + "bar_red.png"));
    }

    // --- SCALING ---
    // Scale X proportionally
    bar->m_Transform.scale.x = maxScale * percent;
    
    // COMPLETELY REMOVED the hardcoded scale.y = 4.0f here so it keeps your constructor values!
    
    // --- POSITIONING ---
    // GetScaledSize already accounts for the new scale.x, so it is the true visual width.
    float exactVisualWidth = bar->GetScaledSize().x;
    
    // Shift the center right by EXACTLY half its new visual width (NO * percent here!)
    bar->m_Transform.translation.x = leftEdgeX + (exactVisualWidth / 2.0f);

    bar->m_Transform.translation.y = fixedY;
}

void BattleUI::ProcessNextMessage() {
    if (m_DialogueQueue.empty()) return;

    // 1. Get the next line of text
    std::string text = m_DialogueQueue.front();
    m_DialogueQueue.pop();
    SetDialogue(text);
    if (text.find("EXP") != std::string::npos || text.find("Exp") != std::string::npos || text.find("gained") != std::string::npos) {
        m_AllowEXPAnimation = true;
    }
    // 2. Figure out who is attacking to trigger the right animation!
    // (If your BattleManager says something other than "used", like "attacks with", change this string!)
    if (text.find("used") != std::string::npos) {
        
        if (text.find(m_PlayerPokemon->GetName()) != std::string::npos) {
            // The text has the Player's name in it, so the Player is attacking!
            m_PlayerLungeTimer = 15;
            m_EnemyShakeTimer = 30;
        } else {
            // Otherwise, the Enemy must be attacking!
            m_EnemyLungeTimer = 15;
            m_PlayerShakeTimer = 30;
        }
    }
}

// In BattleUI.hpp
void BattleUI::StartTrainerBattle(std::vector<std::shared_ptr<Pokemon>> playerParty, 
                                  std::vector<std::shared_ptr<Pokemon>> enemyParty) {
    // 1. Reset battle state
    m_IsTrainerBattle = true; // Flips the rules for this fight!
    
    // 2. Assign the dynamically loaded enemy team
    m_EnemyTeam = enemyParty;
    m_CurrentEnemyIndex = 0;

    // Safety check just in case the JSON data was empty or failed to load
    if (m_EnemyTeam.empty()) {
        LOG_ERROR("Enemy team is empty! Aborting battle setup.");
        return;
    }

    // 3. Set the active enemy to the first Pokemon in the team
    m_EnemyPokemon = m_EnemyTeam[m_CurrentEnemyIndex];

    // 4. Trigger your visual setup by reusing your existing Show() function!
    Show(playerParty, m_EnemyPokemon); 
}




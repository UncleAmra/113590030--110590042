#pragma once
#include "pch.hpp"
#include "Pokemon.hpp"
#include "BattleManager.hpp"
#include "Util/GameObject.hpp"
#include "Util/Renderer.hpp"
#include "ResourceManager.hpp"
#include "Util/Text.hpp"
#include <memory>
#include <vector>

class BattleUI {
public:
    BattleUI(std::shared_ptr<Util::Renderer> renderer);
    void Show(std::vector<std::shared_ptr<Pokemon>> playerParty, std::shared_ptr<Pokemon> wildPokemon);
    void Hide();
    bool Update();

private:

    enum class UIState {
            ANIMATING,
            MAIN_MENU,
            MOVE_MENU,
            WAITING_TEXT
        };

    UIState m_UIState = UIState::ANIMATING;
    int m_CursorIndex = 0;
    void UpdateCursorPosition();
    
    std::shared_ptr<Util::Renderer> m_Renderer;
    std::unique_ptr<BattleManager> m_BattleLogic;
    std::shared_ptr<Util::GameObject> m_PlayerBase;
    std::shared_ptr<Util::GameObject> m_EnemyBase;
    bool m_IsVisible = false;

    // ==========================================
    // 1. BACKGROUND & SPRITES
    // ==========================================
    std::shared_ptr<Util::GameObject> m_Background;
    std::shared_ptr<Util::GameObject> m_PlayerSprite;
    std::shared_ptr<Util::GameObject> m_EnemySprite;

    // ==========================================
    // 2. HP BARS & INFO PANELS
    // ==========================================
    std::shared_ptr<Util::GameObject> m_PlayerPanel;
    std::shared_ptr<Util::Text> m_PlayerNameText;
    std::shared_ptr<Util::Text> m_PlayerHPText;

    std::shared_ptr<Util::GameObject> m_EnemyPanel;
    std::shared_ptr<Util::Text> m_EnemyNameText;
    

    // ==========================================
    // 3. DIALOGUE & COMMAND MENU
    // ==========================================
    std::shared_ptr<Util::GameObject> m_DialogueBox;
    std::shared_ptr<Util::Text> m_DialogueText;

    std::shared_ptr<Util::GameObject> m_CommandBox; 
    std::shared_ptr<Util::GameObject> m_MenuCursor;
    // (We will add the actual health bar fill later once the text works!)

    int m_IntroAnimTimer = 0;
    bool m_IsIntroAnimating = false;

    std::shared_ptr<Util::Image> m_EnemyFrame1;
    std::shared_ptr<Util::Image> m_EnemyFrame2;
    std::shared_ptr<Util::Image> m_PlayerFrame1;
    std::shared_ptr<Util::Image> m_PlayerFrame2;

    int m_CommandCursorIndex = 0;
    bool m_IsSlidingIn = false;
    int m_SlideTimer = 0;
};
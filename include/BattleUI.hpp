#pragma once
#include "pch.hpp"
#include "Pokemon.hpp"
#include "BattleManager.hpp"
#include "Util/GameObject.hpp"
#include "Util/Renderer.hpp"
#include "ResourceManager.hpp"
#include "Util/Text.hpp"
#include <memory>
#include <cmath>   // For std::sin
#include <cstdlib> // For rand()
#include <vector>
#include <queue>      
#include <sstream>

class BattleUI {
public:
    BattleUI(std::shared_ptr<Util::Renderer> renderer);
    void Show(std::vector<std::shared_ptr<Pokemon>> playerParty, std::shared_ptr<Pokemon> wildPokemon);
    void Hide();
    bool Update();
    bool IsBattleOver() const { return m_BattleOver; }

private:

    enum class UIState {
            ANIMATING,
            MAIN_MENU,
            MOVE_MENU,
            WAITING_TEXT
            //BATTLE_ESCAPED
        };
    std::shared_ptr<Pokemon> m_PlayerPokemon;
    std::shared_ptr<Pokemon> m_EnemyPokemon;
    bool m_EscapeSuccessful = false;

    UIState m_UIState = UIState::ANIMATING;
    int m_CursorIndex = 0;
    void UpdateCursorPosition();
    void UpdateMenuVisibility();    
    void SetDialogue(const std::string& text);    
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

    // Keep the GameObjects you already have:
    std::shared_ptr<Util::GameObject> m_PlayerLevelText;
    std::shared_ptr<Util::GameObject> m_EnemyLevelText;

    // ADD THESE: Direct pointers to the text components
    std::shared_ptr<Util::Text> m_PlayerLevelTextDrawable;
    std::shared_ptr<Util::Text> m_EnemyLevelTextDrawable;

        // UI GameObjects for the bars and text
    std::shared_ptr<Util::GameObject> m_PlayerHPBar;
    std::shared_ptr<Util::GameObject> m_EnemyHPBar;
    std::shared_ptr<Util::GameObject> m_PlayerEXPBar;
    //std::shared_ptr<Util::Text> m_PlayerHPText;
    std::shared_ptr<Util::GameObject> m_PlayerHPTextObj;

    // ==========================================
    // 3. DIALOGUE & COMMAND MENU
    // ==========================================
    std::shared_ptr<Util::GameObject> m_DialogueBox;
    std::shared_ptr<Util::Text> m_DialogueText;
    std::shared_ptr<Util::GameObject> m_DialogueTextObj;
    std::shared_ptr<Util::Text> m_MoveTexts[4];
    std::shared_ptr<Util::GameObject> m_MoveTextObjs[4];

    std::shared_ptr<Util::GameObject> m_CommandBox; 
    std::shared_ptr<Util::GameObject> m_MenuCursor;
    std::shared_ptr<Util::GameObject> m_MoveBox;
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
    int m_TextWaitTimer = 0;

    int m_AnimTime = 0;         // For the continuous breathing sine wave
    int m_PlayerLungeTimer = 0; // For the attack lunge
    int m_EnemyShakeTimer = 0;

    std::queue<std::string> m_DialogueQueue;
    int m_EnemyLungeTimer = 0;
    int m_PlayerShakeTimer = 0;

    float m_DisplayPlayerHPPercent = 1.0f;
    float m_DisplayEnemyHPPercent = 1.0f;
    float m_DisplayPlayerEXPPercent = 0.0f;
    bool m_AllowEXPAnimation = false;
    // Add this under your private/protected functions:
    void ProcessNextMessage();

    bool m_BattleOver = false;
    BattleManager::TurnResult m_LastResult;


    // Helper to scale bars cleanly from the left side
    void UpdateBar(std::shared_ptr<Util::GameObject> bar, float percent, float leftEdgeX, float fixedY, float maxScale, bool isHPBar);
};
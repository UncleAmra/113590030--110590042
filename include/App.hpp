#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp"
#include "Player.hpp"
#include "StartMenu.hpp"
#include "InventoryMenu.hpp"
#include "Map.hpp"
#include "GameConfig.hpp"
#include "util/Text.hpp"

// TODO: Implement this later for Battle triggers and special NPC events
struct InteractionResult {
    std::vector<std::string> dialogueLines; 
    std::string specialAction;              
};

enum class MenuState {
    CLOSED,
    START_MENU,  // The right-side menu (Pokedex, Pokemon, Bag, Save, etc.)
    BAG_MAIN     // The actual inventory screen
};
class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }
    MenuState m_MenuState = MenuState::CLOSED;
    
    // Start Menu UI Elements
    std::shared_ptr<Util::GameObject> m_StartMenuBoxUI;
    std::shared_ptr<Util::GameObject> m_StartMenuCursorUI;
    std::shared_ptr<Util::GameObject> m_StartMenuTextUI;// Holds the text bloc
    void Start();
    void Update();
    void End();
    std::vector<std::string> m_CurrentDialogueLines;
    size_t m_CurrentDialogueIndex = 0;
    std::shared_ptr<StartMenu> m_StartMenu;
    
private:
    void ValidTask();

    std::shared_ptr<Util::Renderer> m_Renderer;
    std::shared_ptr<Map> m_Map;
    std::shared_ptr<Util::GameObject> m_DialogueBoxUI;
    State m_CurrentState = State::START;
    //Player m_Character{0.0f, 0.0f};
    std::shared_ptr<Player> m_Character;
    //bool m_IsIndoors = false;
    bool m_IsInDialogue = false;
    
    // Your UI components
    std::shared_ptr<Util::GameObject> m_DialogueUI;
    std::shared_ptr<Util::Text> m_DialogueText;
    // --- INVENTORY UI ---
    //bool m_IsInventoryOpen = false;
    //std::shared_ptr<Util::GameObject> m_InventoryBoxUI;
    //std::shared_ptr<Util::GameObject> m_InventoryTextUI;
    //std::shared_ptr<Util::Text> m_InventoryText;
    std::shared_ptr<InventoryMenu> m_InventoryMenu;
        // Variables to track where the player's cursor is
    int m_BagTabCursor = 0;    // 0 = Items, 1 = Key Items, 2 = Pokeballs
    int m_BagItemCursor = 0;   // Which item in the list is selected
    int m_BagScrollOffset = 0; // For scrolling down long lists
    int m_StartMenuCursor = 0;
    const int MAX_START_MENU_OPTIONS = 3; // Let's start with 3: Bag, Save, Exit
};

#endif
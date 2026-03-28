#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include "GameConfig.hpp"
#include "util/Text.hpp"

struct InteractionResult {
    std::vector<std::string> dialogueLines; // Multiple lines for pressing 'Z' to advance!
    std::string specialAction;              // e.g., "GIVE_POTION", "START_BATTLE", or "NONE"
};

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();
    void Update();
    void End();
    std::vector<std::string> m_CurrentDialogueLines;
    size_t m_CurrentDialogueIndex = 0;
    
private:
    void ValidTask();

private:
    std::shared_ptr<Map> m_Map;
    std::shared_ptr<Util::GameObject> m_DialogueBoxUI;
    State m_CurrentState = State::START;
    Player m_Character{0.0f, 0.0f};
    bool m_IsIndoors = false;
    bool m_IsInDialogue = false;
    
    // Your UI components
    std::shared_ptr<Util::GameObject> m_DialogueUI;
    std::shared_ptr<Util::Text> m_DialogueText;
};

#endif
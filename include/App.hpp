#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include "GameConfig.hpp"
#include "util/Text.hpp"

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
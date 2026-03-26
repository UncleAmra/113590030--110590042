#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include "GameConfig.hpp"

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
    State m_CurrentState = State::START;
    Player m_Character{0.0f, 0.0f};
    bool m_IsIndoors = false;
};

#endif
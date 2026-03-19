#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "Util/Animation.hpp" // We use Animation now instead of Image
#include <vector>
#include <memory>

class Map;

class Character : public Util::GameObject {
public:
    enum class Direction { DOWN, UP, LEFT, RIGHT };
    enum class State { IDLE, MOVING };

    Character(float x, float y);

    //void Update();
    
    glm::vec2 Update(std::shared_ptr<Map> map);

private:
    //void HandleInput();
    glm::vec2 HandleInput();
    void LoadSprites();
    void UpdateSprite();

    bool m_IsMoving = false;                // Is Red currently between tiles?
    glm::vec2 m_CurrentDirection = {0, 0};  // Which way is he walking?
    float m_PixelsMoved = 0.0f;             // How far has he walked so far?
    
    float m_TileSize = 48.0f;               // 16 pixels * 3.0 scale = 48
    float m_Speed = 4.0f;

    int m_GridX = 6;
    int m_GridY = 6;

    Direction m_Direction = Direction::DOWN;
    State m_State = State::IDLE;

    std::shared_ptr<Util::Animation> m_AnimDown;
    std::shared_ptr<Util::Animation> m_AnimUp;
    std::shared_ptr<Util::Animation> m_AnimLeft;
    std::shared_ptr<Util::Animation> m_AnimRight;
    
    std::shared_ptr<Util::Animation> m_CurrentAnimation;
};

#endif
#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "GameConfig.hpp"
#include "Util/Animation.hpp" 
#include <vector>
#include <memory>

class Map;

class Character : public Util::GameObject {
public:

    enum class Direction { DOWN, UP, LEFT, RIGHT };
    enum class State { IDLE, MOVING };
    
    Character(float x, float y);

    bool HasHitDoor() const { return m_HitDoor; }
    void ClearDoorFlag() { m_HitDoor = false; }
    
    int GetGridX() const { return m_GridX; }
    int GetGridY() const { return m_GridY; }
    void SetGridPosition(int x, int y) { m_GridX = x; m_GridY = y; }
    
    bool IsMoving() const { return m_IsMoving; }
    void StopMoving();
    
    glm::vec2 Update(std::shared_ptr<Map> map);

private:
    glm::vec2 HandleInput();
    void LoadSprites();
    void UpdateSprite();
    
    bool m_HitDoor = false;

    bool m_IsMoving = false;                // Is Red currently between tiles?
    glm::vec2 m_CurrentDirection = {0, 0};  // Which way is he walking?
    float m_PixelsMoved = 0.0f;             // How far has he walked so far?
    
    // We deleted m_TileSize! 
    float m_Speed = 4.0f; // (You could even move this to GameConfig::PLAYER_SPEED if you want!)

    // Default these to 0, App will set the real spawn!
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
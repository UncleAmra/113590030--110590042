#include "Character.hpp"
#include "Map.hpp"

Character::Character(float x, float y) {
    m_Transform.translation = {x, y + 24.0f};
    m_ZIndex = 1.0f;
    m_Visible = true;
    m_State = State::IDLE;
    m_Direction = Direction::DOWN;
    m_Transform.scale = {GameConfig::SCALE, GameConfig::SCALE}; 
    
    // Note: We don't call LoadSprites() here anymore. 
    // The child classes must call it in their own constructors!
}

void Character::UpdateSprite() {
    switch (m_Direction) {
        case Direction::DOWN:  m_CurrentAnimation = m_AnimDown;  break;
        case Direction::UP:    m_CurrentAnimation = m_AnimUp;    break;
        case Direction::LEFT:  m_CurrentAnimation = m_AnimLeft;  break;
        case Direction::RIGHT: m_CurrentAnimation = m_AnimRight; break;
    }
    m_Drawable = m_CurrentAnimation;

    if (m_State == State::MOVING) {
        m_CurrentAnimation->Play(); 
    } else {
        m_CurrentAnimation->Pause();
        m_CurrentAnimation->SetCurrentFrame(0); 
    }
}

bool Character::TryMove(int dx, int dy, std::shared_ptr<Map> map) {
    int targetX = m_GridX + dx;
    int targetY = m_GridY + dy;

    // Visual direction mapping (Up is visually +y, Down is visually -y in OpenGL)
    m_CurrentDirection = {(float)dx, (float)-dy};

    if (map->IsWalkable(targetX, targetY)) {
        m_GridX = targetX;
        m_GridY = targetY;
        m_State = State::MOVING;
        m_IsMoving = true;
        return true;
    } else {
        m_State = State::IDLE;
        m_CurrentDirection = {0.0f, 0.0f};
        return false;
    }
}

glm::vec2 Character::Update(std::shared_ptr<Map> map) {
    glm::vec2 movement = {0.0f, 0.0f};
    float dynamicZ = 0.5f - (m_Transform.translation.y / 1000.0f);
    SetZIndex(dynamicZ);

    if (m_IsMoving) {
        float step = m_Speed;
        if (m_PixelsMoved + step > GameConfig::EFFECTIVE_TILE_SIZE) { 
            step = GameConfig::EFFECTIVE_TILE_SIZE - m_PixelsMoved; 
        }
        m_PixelsMoved += step;
        movement = {m_CurrentDirection.x * step, m_CurrentDirection.y * step};

        if (m_PixelsMoved >= GameConfig::EFFECTIVE_TILE_SIZE) {
            m_IsMoving = false;
            m_PixelsMoved = 0.0f;
            m_CurrentDirection = {0.0f, 0.0f};
            
        }
    }

    UpdateSprite(); 
    return movement; 
}

void Character::StopMoving() {
    m_IsMoving = false;
    m_PixelsMoved = 0.0f;
    m_CurrentDirection = {0.0f, 0.0f};
    m_State = State::IDLE;
}
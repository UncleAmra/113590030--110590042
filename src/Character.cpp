#include "Character.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Map.hpp"
// Removed stdio.h!

Character::Character(float x, float y) {
    m_Transform.translation = {x, y + 24.0f};
    m_ZIndex = 1.0f;
    m_Visible = true;
    m_State = State::IDLE;
    m_Direction = Direction::DOWN;
    m_Transform.scale = {GameConfig::SCALE, GameConfig::SCALE}; // Used config here too!
    
    LoadSprites();
    UpdateSprite();
}

void Character::LoadSprites() {
    std::vector<std::string> downFrames  = {RESOURCE_DIR "/character_000.png", RESOURCE_DIR "/character_001.png", RESOURCE_DIR "/character_000.png", RESOURCE_DIR "/character_002.png"};
    std::vector<std::string> upFrames    = {RESOURCE_DIR "/character_003.png", RESOURCE_DIR "/character_004.png", RESOURCE_DIR "/character_003.png", RESOURCE_DIR "/character_005.png"};
    std::vector<std::string> leftFrames  = {RESOURCE_DIR "/character_006.png", RESOURCE_DIR "/character_007.png", RESOURCE_DIR "/character_006.png", RESOURCE_DIR "/character_008.png"};
    std::vector<std::string> rightFrames = {RESOURCE_DIR "/character_009.png", RESOURCE_DIR "/character_010.png", RESOURCE_DIR "/character_009.png", RESOURCE_DIR "/character_011.png"};

    // Initialize the animations (Paths, Play=false, Interval=150ms, Looping=true, Cooldown=0)
    m_AnimDown  = std::make_shared<Util::Animation>(downFrames, false, 150, true, 0);
    m_AnimUp    = std::make_shared<Util::Animation>(upFrames, false, 150, true, 0);
    m_AnimLeft  = std::make_shared<Util::Animation>(leftFrames, false, 150, true, 0);
    m_AnimRight = std::make_shared<Util::Animation>(rightFrames, false, 150, true, 0);

    // Set default starting state
    m_CurrentAnimation = m_AnimDown;
    m_Drawable = m_CurrentAnimation;
}


void Character::UpdateSprite() {
    // 1. Pick the animation based on the LAST KNOWN direction
    switch (m_Direction) {
        case Direction::DOWN:  m_CurrentAnimation = m_AnimDown;  break;
        case Direction::UP:    m_CurrentAnimation = m_AnimUp;    break;
        case Direction::LEFT:  m_CurrentAnimation = m_AnimLeft;  break;
        case Direction::RIGHT: m_CurrentAnimation = m_AnimRight; break;
    }
    
    m_Drawable = m_CurrentAnimation;

    // 2. Play if moving, or Snap to Stand if idle
    if (m_State == State::MOVING) {
        m_CurrentAnimation->Play(); 
    } else {
        m_CurrentAnimation->Pause();
        // FORCE the flipbook to turn to page 1 (the standing frame)
        m_CurrentAnimation->SetCurrentFrame(0); 
    }
}

glm::vec2 Character::Update(std::shared_ptr<Map> map) {
    glm::vec2 movement = {0.0f, 0.0f};
    float dynamicZ = 0.5f - (m_Transform.translation.y / 1000.0f);
    SetZIndex(dynamicZ);
    
    if (!m_IsMoving) {
        int targetX = m_GridX;
        int targetY = m_GridY;
        bool attemptMove = false;

        if (Util::Input::IsKeyPressed(Util::Keycode::UP) || Util::Input::IsKeyPressed(Util::Keycode::W)) {
            targetY -= 1; 
            m_CurrentDirection = {0.0f, 1.0f};
            m_Direction = Direction::UP;
            attemptMove = true;
        } else if (Util::Input::IsKeyPressed(Util::Keycode::DOWN) || Util::Input::IsKeyPressed(Util::Keycode::S)) {
            targetY += 1;
            m_CurrentDirection = {0.0f, -1.0f};
            m_Direction = Direction::DOWN;
            attemptMove = true;
        } else if (Util::Input::IsKeyPressed(Util::Keycode::LEFT) || Util::Input::IsKeyPressed(Util::Keycode::A)) {
            targetX -= 1;
            m_CurrentDirection = {-1.0f, 0.0f};
            m_Direction = Direction::LEFT;
            attemptMove = true;
        } else if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT) || Util::Input::IsKeyPressed(Util::Keycode::D)) {
            targetX += 1;
            m_CurrentDirection = {1.0f, 0.0f};
            m_Direction = Direction::RIGHT;
            attemptMove = true;
        }
        
        // Indentation fixed!
        if (attemptMove) {
            int groundTile = map->GetTileType(targetX, targetY);
            
            // Ask the map: Is this tile walkable?
            if (map->IsWalkable(targetX, targetY)) {
                // WE ARE WALKING!
                m_GridX = targetX;
                m_GridY = targetY;
                m_State = State::MOVING;
                m_IsMoving = true;
                
            } else {
                // WE BUMPED A WALL! 
                m_State = State::IDLE;
                m_CurrentDirection = {0.0f, 0.0f};
                
                // DOOR LOGIC: Using GameConfig instead of magic numbers 6 and 99!
                if (groundTile == GameConfig::TILE_DOOR || groundTile == GameConfig::TILE_EXIT) {
                    m_HitDoor = true; 
                } else {
                    m_HitDoor = false; 
                }
            }
        } else {
            m_State = State::IDLE;
        }
    }

    // MOVEMENT MATH: Replaced m_TileSize with GameConfig::EFFECTIVE_TILE_SIZE
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
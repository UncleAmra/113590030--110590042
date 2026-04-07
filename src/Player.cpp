#include "Player.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Map.hpp"

Player::Player(float x, float y) : Character(x, y) {
    // 1. Load our specific Red sprites
    m_BaseZIndex = 0.5f;
    LoadSprites();
    // 2. Snap to the first frame
    UpdateSprite(); 
    
}

void Player::LoadSprites() {
    const std::string CHAR_DIR = std::string(RESOURCE_DIR) + "/player/";

    std::vector<std::string> downFrames  = {
        CHAR_DIR + "character_000.png",
        CHAR_DIR + "character_001.png",
        CHAR_DIR + "character_000.png",
        CHAR_DIR + "character_002.png"
    };

    std::vector<std::string> upFrames = {
        CHAR_DIR + "character_003.png",
        CHAR_DIR + "character_004.png",
        CHAR_DIR + "character_003.png",
        CHAR_DIR + "character_005.png"
    };

    std::vector<std::string> leftFrames = {
        CHAR_DIR + "character_006.png",
        CHAR_DIR + "character_007.png",
        CHAR_DIR + "character_006.png",
        CHAR_DIR + "character_008.png"
    };

    std::vector<std::string> rightFrames = {
        CHAR_DIR + "character_009.png",
        CHAR_DIR + "character_010.png",
        CHAR_DIR + "character_009.png",
        CHAR_DIR + "character_011.png"
    };

    m_AnimDown  = std::make_shared<Util::Animation>(downFrames, false, 150, true, 0);
    m_AnimUp    = std::make_shared<Util::Animation>(upFrames, false, 150, true, 0);
    m_AnimLeft  = std::make_shared<Util::Animation>(leftFrames, false, 150, true, 0);
    m_AnimRight = std::make_shared<Util::Animation>(rightFrames, false, 150, true, 0);

    m_CurrentAnimation = m_AnimDown;
    m_Drawable = m_CurrentAnimation;
}

void Player::HandleInput(std::shared_ptr<Map> map) {
    // 1. INPUT LOCK: This MUST be at the very top!
    if (IsMoving()) {
        return; 
    }

    int dx = 0;
    int dy = 0;

    // Set Sprite Animation Directions
    if (Util::Input::IsKeyPressed(Util::Keycode::W) || Util::Input::IsKeyPressed(Util::Keycode::UP)) {
        dy = -1; 
        m_Direction = Direction::UP;
    } else if (Util::Input::IsKeyPressed(Util::Keycode::S) || Util::Input::IsKeyPressed(Util::Keycode::DOWN)) {
        dy = 1;
        m_Direction = Direction::DOWN;
    } else if (Util::Input::IsKeyPressed(Util::Keycode::A) || Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        dx = -1;
        m_Direction = Direction::LEFT;
    } else if (Util::Input::IsKeyPressed(Util::Keycode::D) || Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        dx = 1;
        m_Direction = Direction::RIGHT;
    }

    if (dx != 0 || dy != 0) {
        int targetX = m_GridX + dx;
        int targetY = m_GridY + dy;

        // DOOR LOGIC
        if (!map->IsWalkable(targetX, targetY)) {
            //int groundTile = map->GetTileType(targetX, targetY);
            //if (groundTile == GameConfig::TILE_DOOR || groundTile == GameConfig::TILE_EXIT || Prop == GameConfig::PROP_INVISIBLE_DOOR) {
            //    m_HitDoor = true; 
            //}
            int Prop = map->GetPropType(targetX, targetY);
            if (Prop == GameConfig::PROP_INVISIBLE_DOOR) {
                m_HitDoor = true; 
            }
        }

        // 2. THE MISSING MATH FIX: 
        // We MUST update the movement vector so the camera knows which way to pan!
        m_CurrentDirection = {dx, dy}; 

        TryMove(dx, dy, map);
    }
    else {
    // No key pressed — explicitly go idle
    m_State = State::IDLE;
}
}

glm::vec2 Player::Update(std::shared_ptr<Map> map) {
    HandleInput(map);
    
    // Snapshot moving state BEFORE the base update runs
    bool wasMoving = m_IsMoving;
    
    glm::vec2 movement = Character::Update(map);
    
    map->UpdateSteppedProps(m_GridX, m_GridY);
    
    // If we JUST finished a step this frame, check the tile we landed on
    if (wasMoving && !m_IsMoving) {
        int propID = map->GetPropType(m_GridX, m_GridY);
        
        if (propID == GameConfig::PROP_TALLGRASS) {
            // 15% encounter rate per step — adjust to taste
            if (rand() % 100 < 15) {
                m_WildEncounterTriggered = true;
                LOG_TRACE("Wild encounter triggered at ({}, {})!", m_GridX, m_GridY);
            }
        }
    }
    
    return movement;
}
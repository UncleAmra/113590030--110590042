#include "Player.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Map.hpp"

Player::Player(float x, float y) : Character(x, y) {
    // 1. Load our specific Red sprites
    m_BaseZIndex = 0.8f;
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
    if (m_IsMoving) return;

    int dx = 0;
    int dy = 0;

    if (Util::Input::IsKeyPressed(Util::Keycode::UP) || Util::Input::IsKeyPressed(Util::Keycode::W)) {
        dy = -1; 
        m_Direction = Direction::UP;
    } else if (Util::Input::IsKeyPressed(Util::Keycode::DOWN) || Util::Input::IsKeyPressed(Util::Keycode::S)) {
        dy = 1;
        m_Direction = Direction::DOWN;
    } else if (Util::Input::IsKeyPressed(Util::Keycode::LEFT) || Util::Input::IsKeyPressed(Util::Keycode::A)) {
        dx = -1;
        m_Direction = Direction::LEFT;
    } else if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT) || Util::Input::IsKeyPressed(Util::Keycode::D)) {
        dx = 1;
        m_Direction = Direction::RIGHT;
    }
    

    if (dx != 0 || dy != 0) {
        int targetX = m_GridX + dx;
        int targetY = m_GridY + dy;

        // DOOR LOGIC: Check before we try to move!
        if (!map->IsWalkable(targetX, targetY)) {
            int groundTile = map->GetTileType(targetX, targetY);
            if (groundTile == GameConfig::TILE_DOOR || groundTile == GameConfig::TILE_EXIT) {
                m_HitDoor = true; 
            }
        }

        // Base class handles the math!
        TryMove(dx, dy, map);
    }
    else {
        // ADD THIS: If we aren't pressing keys, the Brain tells the Body to idle!
        m_State = State::IDLE;
    }
}

glm::vec2 Player::Update(std::shared_ptr<Map> map) {
    // 1. Handle our keyboard inputs
    HandleInput(map);
    
    // 2. Let the base Character class do the animation and movement math!
    return Character::Update(map); 
}
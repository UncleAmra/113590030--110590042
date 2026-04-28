#include "BattleAnimator.hpp"
#include <cmath>
#include <cstdlib>
#include <algorithm>

BattleAnimator::BattleAnimator(std::shared_ptr<Util::GameObject> playerSprite,
                               std::shared_ptr<Util::GameObject> enemySprite,
                               std::shared_ptr<Util::Renderer> renderer)
    : m_PlayerSprite(playerSprite), m_EnemySprite(enemySprite) {
    
    // Initialize the effect sprite and add it to the renderer
    m_EffectSprite = std::make_shared<Util::GameObject>();
    m_EffectSprite->SetZIndex(100); // Draw on top of everything
    m_EffectSprite->SetVisible(false);
    
    if (renderer) {
        renderer->AddChild(m_EffectSprite);
    }
}

BattlerVisualState& BattleAnimator::GetState(BattleSide side) {
    return (side == BattleSide::PLAYER) ? m_PlayerState : m_EnemyState;
}

bool BattleAnimator::IsBusy() const {
    return m_DrainingPlayerHP || 
           m_DrainingEnemyHP || 
           m_PlayerShakeTimer > 0.0f || 
           m_EnemyShakeTimer > 0.0f ||
           m_PlayerLungeTimer > 0.0f ||
           m_EnemyLungeTimer > 0.0f ||
           m_ActiveEffect.isPlaying ||
           m_PlayerFainting ||
           m_EnemyFainting;
}

void BattleAnimator::Update(float deltaTime) {
    // Increment base animation time for idle bobbing
    m_AnimTime += 1.0f; 

    // ==========================================
    // 1. UPDATE HP BARS (Smooth Lerp)
    // ==========================================
    if (m_DrainingEnemyHP) {
        if (m_EnemyHPPercent > m_EnemyHPTarget) {
            m_EnemyHPPercent -= m_EnemyHPDrainSpeed; // Use deltaTime if your engine isn't frame-locked
            if (m_EnemyHPPercent <= m_EnemyHPTarget) m_DrainingEnemyHP = false;
        } else if (m_EnemyHPPercent < m_EnemyHPTarget) {
            m_EnemyHPPercent += m_EnemyHPDrainSpeed; // Handle Healing
            if (m_EnemyHPPercent >= m_EnemyHPTarget) m_DrainingEnemyHP = false;
        } else {
             m_DrainingEnemyHP = false; 
        }
    }

    if (m_DrainingPlayerHP) {
        if (m_PlayerHPPercent > m_PlayerHPTarget) {
            m_PlayerHPPercent -= m_PlayerHPDrainSpeed;
            if (m_PlayerHPPercent <= m_PlayerHPTarget) m_DrainingPlayerHP = false;
        } else if (m_PlayerHPPercent < m_PlayerHPTarget) {
            m_PlayerHPPercent += m_PlayerHPDrainSpeed; // Handle Healing
            if (m_PlayerHPPercent >= m_PlayerHPTarget) m_DrainingPlayerHP = false;
        } else {
            m_DrainingPlayerHP = false;
        }
    }

    // Ensure HP never goes out of bounds
    m_EnemyHPPercent = std::clamp(m_EnemyHPPercent, 0.0f, 1.0f);
    m_PlayerHPPercent = std::clamp(m_PlayerHPPercent, 0.0f, 1.0f);

    // ==========================================
    // 2. UPDATE ACTION OFFSETS (Lunges & Shakes)
    // ==========================================
    // Player Shaking / Lunging
    if (m_PlayerLungeTimer > 0.0f) {
        m_PlayerLungeTimer -= 1.0f;
        m_PlayerState.offsetX = 40.0f; // Jump forward
    } else if (m_PlayerShakeTimer > 0.0f) {
        m_PlayerShakeTimer -= 1.0f;
        m_PlayerState.offsetX = (rand() % (int)(m_PlayerShakeIntensity * 2)) - m_PlayerShakeIntensity;
    } else {
        m_PlayerState.offsetX = 0.0f;
    }

    // Enemy Shaking / Lunging
    if (m_EnemyLungeTimer > 0.0f) {
        m_EnemyLungeTimer -= 1.0f;
        m_EnemyState.offsetX = -40.0f; // Jump forward (left)
    } else if (m_EnemyShakeTimer > 0.0f) {
        m_EnemyShakeTimer -= 1.0f;
        m_EnemyState.offsetX = (rand() % (int)(m_EnemyShakeIntensity * 2)) - m_EnemyShakeIntensity;
    } else {
        m_EnemyState.offsetX = 0.0f;
    }

    // ==========================================
    // 3. UPDATE FAINTING
    // ==========================================
    if (m_EnemyFainting) {
        m_EnemyState.offsetY -= 5.0f; // Changed to minus!
        if (m_EnemyState.offsetY < -150.0f) { // Check for negative!
            m_EnemySprite->SetVisible(false);
            m_EnemyFainting = false;
        }
    }

    if (m_PlayerFainting) {
        m_PlayerState.offsetY -= 5.0f; // Changed to minus!
        if (m_PlayerState.offsetY < -150.0f) { // Check for negative!
            m_PlayerSprite->SetVisible(false);
            m_PlayerFainting = false;
        }
    }

    // ==========================================
    // 4. UPDATE ATTACK SPRITESHEET
    // ==========================================
    if (m_ActiveEffect.isPlaying) {
        m_ActiveEffect.frameTimer += deltaTime; // Use standard time here if possible
        if (m_ActiveEffect.frameTimer >= m_ActiveEffect.timePerFrame) {
            m_ActiveEffect.frameTimer = 0.0f;
            m_ActiveEffect.currentFrame++;

            if (m_ActiveEffect.currentFrame >= m_ActiveEffect.totalFrames) {
                m_ActiveEffect.isPlaying = false;
                m_EffectSprite->SetVisible(false);
            } else {
                // TODO: Update UV rect or switch texture for m_EffectSprite based on currentFrame
                // m_EffectSprite->SetDrawable(...);
            }
        }
    }

    // ==========================================
    // 5. APPLY FINAL TRANSFORMS TO GAMEOBJECTS
    // ==========================================
    // Add procedural bobbing directly onto the base + action offsets
    float playerBob = std::sin(m_AnimTime * 0.04f) * 2.5f; 
    float enemyBob = std::sin(m_AnimTime * 0.03f) * 1.5f;

    if (m_PlayerSprite) {
        m_PlayerSprite->m_Transform.translation.x = PLAYER_BASE_X + m_PlayerState.offsetX;
        m_PlayerSprite->m_Transform.translation.y = PLAYER_BASE_Y + playerBob + m_PlayerState.offsetY;
    }

    if (m_EnemySprite) {
        m_EnemySprite->m_Transform.translation.x = ENEMY_BASE_X + m_EnemyState.offsetX;
        m_EnemySprite->m_Transform.translation.y = ENEMY_BASE_Y + enemyBob + m_EnemyState.offsetY;
    }
}

// ==========================================
// COMMAND TRIGGERS
// ==========================================
void BattleAnimator::ShakeSprite(BattleSide side, float duration, float intensity) {
    if (side == BattleSide::PLAYER) {
        m_PlayerShakeTimer = duration;
        m_PlayerShakeIntensity = intensity;
    } else {
        m_EnemyShakeTimer = duration;
        m_EnemyShakeIntensity = intensity;
    }
}

void BattleAnimator::LungeSprite(BattleSide side, float duration) {
    if (side == BattleSide::PLAYER) m_PlayerLungeTimer = duration;
    else m_EnemyLungeTimer = duration;
}

void BattleAnimator::AnimateHPDrain(BattleSide side, float startPercent, float targetPercent, float speed) {
    if (side == BattleSide::PLAYER) {
        m_PlayerHPPercent = startPercent;
        m_PlayerHPTarget = targetPercent;
        m_PlayerHPDrainSpeed = speed;
        m_DrainingPlayerHP = true;
    } else {
        m_EnemyHPPercent = startPercent;
        m_EnemyHPTarget = targetPercent;
        m_EnemyHPDrainSpeed = speed;
        m_DrainingEnemyHP = true;
    }
}

void BattleAnimator::PlayFaint(BattleSide side) {
    if (side == BattleSide::PLAYER) m_PlayerFainting = true;
    else m_EnemyFainting = true;
}

void BattleAnimator::PlayAttackEffect(const std::string& texturePath, int frames, BattleSide target) {
    m_ActiveEffect.isPlaying = true;
    m_ActiveEffect.texturePath = texturePath;
    m_ActiveEffect.totalFrames = frames;
    m_ActiveEffect.currentFrame = 0;
    m_ActiveEffect.frameTimer = 0.0f;
    m_ActiveEffect.targetSide = target;

    m_EffectSprite->SetVisible(true);
    
    // Position the effect exactly over the target
    if (target == BattleSide::ENEMY) {
        m_EffectSprite->m_Transform.translation = { ENEMY_BASE_X, ENEMY_BASE_Y };
    } else {
        m_EffectSprite->m_Transform.translation = { PLAYER_BASE_X, PLAYER_BASE_Y };
    }
}

void BattleAnimator::ResetState(BattleSide side) {
    BattlerVisualState& state = GetState(side);
    
    // 1. Reset all visual offsets back to 0
    state.offsetX = 0.0f;
    state.offsetY = 0.0f;
    state.opacity = 1.0f;
    state.isVisible = true;

    // 2. Kill any lingering animations for this side
    if (side == BattleSide::PLAYER) {
        m_PlayerFainting = false;
        m_PlayerLungeTimer = 0.0f;
        m_PlayerShakeTimer = 0.0f;
        if (m_PlayerSprite) m_PlayerSprite->SetVisible(true);
    } else {
        m_EnemyFainting = false;
        m_EnemyLungeTimer = 0.0f;
        m_EnemyShakeTimer = 0.0f;
        if (m_EnemySprite) m_EnemySprite->SetVisible(true);
    }
}
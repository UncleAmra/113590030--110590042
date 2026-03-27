#include "NPC.hpp"

// 1. Initialize m_UseDynamicZ to false by default
NPC::NPC(float x, float y, const std::string& spritePath) 
    : Character(x, y), m_SpritePath(spritePath), m_UseDynamicZ(false) {
    
    // We must call these here so the NPC gets its visual components!
    LoadSprites();
    UpdateSprite();
}

void NPC::LoadSprites() {
    // For now, we just use the same image for all 4 directions
    std::vector<std::string> frames = {m_SpritePath};
    
    m_AnimDown  = std::make_shared<Util::Animation>(frames, false, 150, true, 0);
    m_AnimUp    = std::make_shared<Util::Animation>(frames, false, 150, true, 0);
    m_AnimLeft  = std::make_shared<Util::Animation>(frames, false, 150, true, 0);
    m_AnimRight = std::make_shared<Util::Animation>(frames, false, 150, true, 0);

    m_CurrentAnimation = m_AnimDown;
    m_Drawable = m_CurrentAnimation;
}

glm::vec2 NPC::Update(std::shared_ptr<Map> map) {
    //if (!map) return {0.0f, 0.0f}; 
    
    // We deleted the math! The parent class handles the Y-sorting perfectly now.
    return Character::Update(map); 
}
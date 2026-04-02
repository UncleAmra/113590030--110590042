#include "Prop.hpp" 
#include "ResourceManager.hpp" 
#include "GameConfig.hpp"

Prop::Prop(const std::string& imagePath, const glm::vec2& startPosition, float scale, float zIndex, int gridX, int gridY, const std::string& altImagePath) {
    
    // 1. Setup the basic transform stuff (yours might look slightly different)
    m_Transform.translation = startPosition;
    m_Transform.scale = {scale, scale};
    SetZIndex(zIndex);

    // 2. Load the normal image
    m_DefaultImage = std::make_shared<Util::Image>(imagePath);
    SetDrawable(m_DefaultImage);

    // 3. THE CRUCIAL PART: Actually save the grid coordinates!
    m_GridX = gridX;
    m_GridY = gridY;

    // 4. THE OTHER CRUCIAL PART: Load the squished image if one exists!
    if (!altImagePath.empty()) {
        m_AltImage = std::make_shared<Util::Image>(altImagePath);
    } else {
        m_AltImage = nullptr; // Just to be safe
    }
}
void Prop::SetSteppedOn(bool stepped) {
    if (stepped && m_AltImage != nullptr) {
        // Switch to the flattened grass graphic
        SetDrawable(m_AltImage); 
    } else if (!stepped && m_DefaultImage != nullptr) {
        // Revert back to the normal graphic when they step off
        SetDrawable(m_DefaultImage); 
    }
}

void Prop::Update() {
    if (m_UseDynamicZ) {
        float footY = m_Transform.translation.y - (GameConfig::SCALED_TILE_SIZE * 0.5f);
        float dynamicZ = 0.5f - (footY / 10000.0f);
        SetZIndex(dynamicZ);
    }
}
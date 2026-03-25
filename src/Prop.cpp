#include "Prop.hpp" // Don't forget to include your new header!

Prop::Prop(const std::string& imagePath, const glm::vec2& startPosition, float scale, float zIndex) {
    auto image = std::make_shared<Util::Image>(imagePath);
    SetDrawable(image);
    m_Transform.translation = startPosition;
    m_Transform.scale = glm::vec2(scale, scale);
    SetZIndex(zIndex);
}

void Prop::Update() {
    // Y-Sorting Magic!
    // We start at a base of 0.5f (above the ground tiles at 0.0f).
    // We divide Y by 1000.0f to keep the number small enough that it doesn't break the engine.
    if (m_UseDynamicZ) {
        float dynamicZ = 0.5f - (m_Transform.translation.y / 1000.0f);
        SetZIndex(dynamicZ);
    }
}
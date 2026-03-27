#include "Prop.hpp" // Don't forget to include your new header!

Prop::Prop(const std::string& imagePath, const glm::vec2& startPosition, float scale, float zIndex) {
    auto image = std::make_shared<Util::Image>(imagePath);
    SetDrawable(image);
    m_Transform.translation = startPosition;
    m_Transform.scale = glm::vec2(scale, scale);
    SetZIndex(zIndex);
}

void Prop::Update() {
    if (m_UseDynamicZ) {
        float dynamicZ = 0.8f - (m_Transform.translation.y / 10000.0f);
        SetZIndex(dynamicZ);
    }
}
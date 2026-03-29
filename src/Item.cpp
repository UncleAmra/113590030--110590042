#include "Item.hpp"

Item::Item(const std::string& imagePath, const glm::vec2& position, const std::string& name, int gridX, int gridY) 
    : m_Name(name), m_GridX(gridX), m_GridY(gridY) {
    
    // 1. Load the texture
    m_Drawable = std::make_shared<Util::Image>(imagePath);
    
    // 2. Set the position
    m_Transform.translation = position;
    m_Transform.scale = glm::vec2(3.0f, 3.0f);
    // Items usually sit below the player, so a low Z-Index is good
    m_ZIndex = 0.1f; 
}

void Item::Collect() {
    // When collected, we flag it. 
    // Later, the Map will see this flag and delete the item from the ground!
    m_IsCollected = true;
    m_Visible = false; // Instantly hide it from the screen
}
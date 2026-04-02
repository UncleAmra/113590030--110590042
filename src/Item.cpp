#include "Item.hpp"
#include "ResourceManager.hpp" 

Item::Item(const std::string& imagePath, const glm::vec2& position, const std::string& name, ItemCategory category, int gridX, int gridY) 
    : m_Name(name), m_GridX(gridX), m_GridY(gridY) {
    
    // 1. Fetch the texture from the Manager instead of the hard drive
    m_Drawable = ResourceManager::GetImageStore().Get(imagePath);
    
    // 2. Set the position
    m_Transform.translation = position;
    m_Transform.scale = glm::vec2(3.0f, 3.0f);
    m_Category = category;
    
    // Items usually sit below the player, so a low Z-Index is good
    m_ZIndex = 0.1f; 
}

void Item::Collect() {
    // When collected, we flag it. 
    // Later, the Map will see this flag and delete the item from the ground!
    m_IsCollected = true;
    m_Visible = false; // Instantly hide it from the screen
}
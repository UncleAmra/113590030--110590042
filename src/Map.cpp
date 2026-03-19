#include "Map.hpp"

std::shared_ptr<Map> m_Map;

Map::Map(const std::string& imagePath) {
    m_Image = std::make_shared<Util::Image>(imagePath);
    m_Drawable = m_Image;
    
    // Z-Index of 0.0f ensures the map renders BEHIND the player (who is at 1.0f)
    m_ZIndex = 0.0f; 
    
    // Optional: If your map is tiny pixel art, scale it up just like the player!
    m_Transform.scale = {3.0f, 3.0f}; 
    
    // Start the map centered
    m_Transform.translation = {0.0f, 0.0f}; 
}

void Map::Move(float dx, float dy) {
    m_Transform.translation.x += dx;
    m_Transform.translation.y += dy;
}
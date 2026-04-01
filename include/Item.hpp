#ifndef ITEM_HPP
#define ITEM_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <string>
#include <memory>

enum class ItemCategory {
    GENERAL = 0,
    POKEBALLS = 1,
    KEY_ITEMS = 2,
    COUNT = 3 
};

class Item : public Util::GameObject {
public:
    // Update the constructor to take the category as an argument
    Item(const std::string& imagePath, const glm::vec2& position, const std::string& name, ItemCategory category, int gridX, int gridY);

    std::string GetName() const { return m_Name; }
    
    // Standard getter instead of a static helper!
    ItemCategory GetCategory() const { return m_Category; }
    
    void Collect(); 

    bool IsCollected() const { return m_IsCollected; }
    int GetGridX() const { return m_GridX; }
    int GetGridY() const { return m_GridY; }

private:
    std::string m_Name;
    ItemCategory m_Category; // <-- Now it is a proper attribute
    bool m_IsCollected = false;
    int m_GridX; 
    int m_GridY;
};

#endif
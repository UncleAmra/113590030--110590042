#ifndef ITEM_HPP
#define ITEM_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <string>
#include <memory>

class Item : public Util::GameObject {
public:
    // The constructor takes the image path, position, and the item's name!
    Item(const std::string& imagePath, const glm::vec2& position, const std::string& name, int gridX, int gridY);


    // Getters so the player's inventory knows what they just picked up
    std::string GetName() const { return m_Name; }
    
    // A function we can call when the player steps on it!
    void Collect(); 

    bool IsCollected() const { return m_IsCollected; }
    int GetGridX() const { return m_GridX; }
    int GetGridY() const { return m_GridY; }

private:
    std::string m_Name;
    bool m_IsCollected = false;
    int m_GridX; // <-- Add this
    int m_GridY;
};

#endif
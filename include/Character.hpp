#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "GameConfig.hpp"
#include "Util/Animation.hpp" 
#include <vector>
#include <memory>

class Map;

class Character : public Util::GameObject {
public:
    enum class Direction { DOWN, UP, LEFT, RIGHT };
    enum class State { IDLE, MOVING };
    
    Character(float x, float y);
    virtual ~Character() = default; // Good practice for base classes!

    int GetGridX() const { return m_GridX; }
    int GetGridY() const { return m_GridY; }
    void SetGridPosition(int x, int y) { m_GridX = x; m_GridY = y; }
    
    bool IsMoving() const { return m_IsMoving; }
    void StopMoving();
    void SetBaseZIndex(float z) { m_BaseZIndex = z; }
    // Virtual so Player can add input logic to it!
    virtual glm::vec2 Update(std::shared_ptr<Map> map);
    void SetDirection(Direction dir);
    Direction GetFacingDirection() const { return m_Direction; }

    // --- NEW INVENTORY SYSTEM ---
    void AddItem(const std::string& itemName, int amount = 1);
    bool RemoveItem(const std::string& itemName, int amount = 1);
    int GetItemCount(const std::string& itemName) const;
    void PrintInventory() const; // Great for debugging!
    std::unordered_map<std::string, int> GetInventory() const { return m_Inventory; }
    void SetInventory(const std::unordered_map<std::string, int>& loadedInv) { m_Inventory = loadedInv; }


protected:
    // The Backpack: Maps the Item's Name to the Quantity they own
    std::unordered_map<std::string, int> m_Inventory;
    // Pure virtual function! Forces Player and NPC to load their own unique sprites.
    virtual void LoadSprites() = 0; 
    bool m_UseDynamicZ = true;
    void UpdateSprite();
    float m_BaseZIndex = 0.0f;
    
    // A generic function to try and walk into a tile
    bool TryMove(int dx, int dy, std::shared_ptr<Map> map);

    bool m_IsMoving = false;                
    glm::vec2 m_CurrentDirection = {0, 0};  
    float m_PixelsMoved = 0.0f;             
    float m_Speed = 200.0f; 

    int m_GridX = 6;
    int m_GridY = 6;

    Direction m_Direction = Direction::DOWN;
    State m_State = State::IDLE;

    std::shared_ptr<Util::Animation> m_AnimDown;
    std::shared_ptr<Util::Animation> m_AnimUp;
    std::shared_ptr<Util::Animation> m_AnimLeft;
    std::shared_ptr<Util::Animation> m_AnimRight;
    std::shared_ptr<Util::Animation> m_CurrentAnimation;

    
};

#endif
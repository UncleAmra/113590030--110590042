#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Character.hpp"
#include "Item.hpp"

class Player : public Character {
public:
    Player(float x, float y);

    // We override the base update to add our input logic!
    glm::vec2 Update(std::shared_ptr<Map> map) override;

    bool HasHitDoor() const { return m_HitDoor; }
    void ClearDoorFlag() { m_HitDoor = false; }
    void HandleInput(std::shared_ptr<Map> map);
protected:
    void LoadSprites() override; // Loads the Red character sprites

private:
    bool m_HitDoor = false;
};

#endif
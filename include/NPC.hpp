#ifndef NPC_HPP
#define NPC_HPP

#include "Character.hpp"
#include <string>

class NPC : public Character {
public:
    NPC(float x, float y, const std::string& spritePath);
    glm::vec2 Update(std::shared_ptr<Map>) override;

protected:
    void LoadSprites() override;

private:
    std::string m_SpritePath;
};

#endif
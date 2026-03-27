#ifndef NPC_HPP
#define NPC_HPP

#include "Character.hpp"
#include <string>

class NPC : public Character {
public:
    NPC(float x, float y, const std::string& spritePath, const std::string& dialoguePath);
    glm::vec2 Update(std::shared_ptr<Map>) override;
    void SetDynamicZ(bool dynamic) { m_UseDynamicZ = dynamic; }
    std::string Interact();

protected:
    void LoadSprites() override;

private:
    std::string m_SpritePath;
    bool m_UseDynamicZ;
    std::string m_Dialogue;
};

#endif
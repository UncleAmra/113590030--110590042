#ifndef NPC_HPP
#define NPC_HPP

#include "Character.hpp"
#include <string>
#include <vector> // Make sure vector is included

class NPC : public Character {
public:
    // 1. Updated constructor with default empty strings for the new features
    NPC(float x, float y, const std::string& spritePath, 
        const std::string& dialoguePath, 
        const std::string& altDialoguePath = "", 
        const std::string& flagCondition = "");

    glm::vec2 Update(std::shared_ptr<Map>) override;
    void SetDynamicZ(bool dynamic) { m_UseDynamicZ = dynamic; }
    
    std::vector<std::string> Interact();

protected:
    void LoadSprites() override;

private:
    std::string m_SpritePath;
    bool m_UseDynamicZ;
    
    // 2. Changed m_Dialogue to a vector called m_DialogueLines
    std::vector<std::string> m_DialogueLines;       
    std::vector<std::string> m_AltDialogueLines;    
    std::string m_FlagCondition;                    
};

#endif
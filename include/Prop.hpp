#pragma once
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <string>
#include <vector> // <-- Add this!
#include <memory>
#include "Map.hpp" 



class Prop : public Util::GameObject {
public:
    // UPDATE CONSTRUCTOR: Take a vector of strings instead of 1 or 2 strings
    Prop(const std::vector<std::string>& imagePaths, const glm::vec2& startPosition, float scale = 3.0f, float zIndex = 0.0f, int gridX = -1, int gridY = -1);

    void Update();

    int GetGridX() const { return m_GridX; }
    int GetGridY() const { return m_GridY; }

    // NEW: Flexible state changing
    void SetState(int stateIndex);
    int GetState() const { return m_CurrentState; }

    // We can keep this so your grass logic doesn't break!
    void SetSteppedOn(bool stepped); 
    
    void SetDynamicZ(bool useDynamic) { m_UseDynamicZ = useDynamic; }
    void SetAnimMode(PropAnimMode mode, int frameDelay = 8);
private:
    // REPLACED: m_DefaultImage and m_AltImage are now a list!
    std::vector<std::shared_ptr<Util::Image>> m_Images;
    int m_CurrentState = 0;
    int m_TargetState = 0;
    int m_FrameDelayCounter = 0;
    int m_GridX = -1;
    int m_GridY = -1;
    bool m_IsSteppedOn = false;
    bool m_UseDynamicZ = false;
    float m_BaseZIndex = 0.0f; // Add this line to remember the original Z!
    PropAnimMode m_AnimMode = PropAnimMode::STATIC;
    int m_AnimFrameDelay = 8;
    int m_AnimCounter = 0;
    int m_AnimDirection = 1; // 1 = forward, -1 = backward (for ping-pong)
};
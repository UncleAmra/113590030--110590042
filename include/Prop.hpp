#ifndef PROP_HPP
#define PROP_HPP
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <string>
#include <glm/glm.hpp>

class Prop : public Util::GameObject {
public:
    // Make sure 'const', '&', and the default values are exactly like this!
// Add the new variables to the end with defaults!
    Prop(const std::string& imagePath, const glm::vec2& startPosition, float scale = 3.0f, float zIndex = 0.0f, int gridX = -1, int gridY = -1, const std::string& altImagePath = "");    void Update();
    void SetSteppedOn(bool stepped);
    void SetDynamicZ(bool useDynamic) { m_UseDynamicZ = useDynamic; }
    int GetGridX() const { return m_GridX; }
    int GetGridY() const { return m_GridY; }

private: 
    std::shared_ptr<Util::Image> m_DefaultImage;
    std::shared_ptr<Util::Image> m_AltImage; // The stepped-on version
    bool m_IsSteppedOn = false;
    bool m_UseDynamicZ = true;
    int m_GridX;
    int m_GridY;
};



#endif // PROP_HPP
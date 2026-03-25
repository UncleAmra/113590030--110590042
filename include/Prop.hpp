#ifndef PROP_HPP
#define PROP_HPP
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <string>
#include <glm/glm.hpp>

class Prop : public Util::GameObject {
public:
    // Make sure 'const', '&', and the default values are exactly like this!
    Prop(const std::string& imagePath, const glm::vec2& startPosition, float scale = 3.0f, float zIndex = 0.5f);

    void Update();
};

#endif // PROP_HPP
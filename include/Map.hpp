#ifndef MAP_HPP
#define MAP_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <memory>
#include <string>

class Map : public Util::GameObject {
public:
    Map(const std::string& imagePath);

    // We will use this function to shift the world around the player
    void Move(float dx, float dy);

private:
    std::shared_ptr<Util::Image> m_Image;
};

#endif
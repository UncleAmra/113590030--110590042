#ifndef MAP_HPP
#define MAP_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <vector>
#include <memory>

class Map : public Util::GameObject {
public:
    Map(); 
    void Move(float dx, float dy);
    void Draw(); 
    bool IsWalkable(int x, int y);

private:
    // Now we just hold a list of GameObjects!
    std::vector<std::shared_ptr<Util::GameObject>> m_Tiles;
    std::vector<std::vector<int>> m_LevelData;
    std::shared_ptr<Util::Image> m_GrassImage;
    std::shared_ptr<Util::Image> m_WaterImage;
    std::shared_ptr<Util::Image> m_DirtImage;
    std::shared_ptr<Util::Image> m_PokeCentreImage;
    std::shared_ptr<Util::Image> m_ChurchImage;  
    
    void LoadMapFromFile(const std::string& filepath);
    
};

#endif
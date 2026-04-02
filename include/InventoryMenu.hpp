#pragma once

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include "Util/Renderer.hpp"
#include "Item.hpp" 
#include <memory>
#include <string>
#include <vector>
#include <map>

class InventoryMenu {
public:
    InventoryMenu(std::shared_ptr<Util::Renderer> renderer);
    ~InventoryMenu() = default;

    // Accepts the pre-sorted categories
    void Show(const std::map<ItemCategory, std::vector<std::pair<std::string, int>>>& categorizedItems);
    
    void Hide();
    bool Update(); 

private:
    void UpdateText();

    std::shared_ptr<Util::GameObject> m_BoxUI;
    std::shared_ptr<Util::GameObject> m_TextUI;
    std::shared_ptr<Util::Text> m_Text;

    // Scrolling & Tab state variables
    std::map<ItemCategory, std::vector<std::pair<std::string, int>>> m_CategorizedItems;
    ItemCategory m_CurrentTab = ItemCategory::GENERAL;
    
    int m_SelectedIndex = 0;
    int m_ScrollOffset = 0;
    const int MAX_VISIBLE_ITEMS = 6; 
};  
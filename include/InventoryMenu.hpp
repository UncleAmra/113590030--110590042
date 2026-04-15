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

    // Returns the name of the currently highlighted item (or "" if empty)
    std::string GetSelectedItem() const {
        if (m_CategorizedItems.count(m_CurrentTab) == 0) return "";
        const auto& list = m_CategorizedItems.at(m_CurrentTab);
        if (list.empty() || m_SelectedIndex >= static_cast<int>(list.size())) return "";
        return list[m_SelectedIndex].first;
    }

    // Returns the current tab so we can prevent players from throwing bicycles at Pokemon
    ItemCategory GetCurrentTab() const { 
        return m_CurrentTab; 
    }

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
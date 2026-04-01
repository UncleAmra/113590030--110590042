#pragma once

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include "Util/Renderer.hpp"
#include <memory>
#include <string>
#include <vector>

class InventoryMenu {
public:
    InventoryMenu(std::shared_ptr<Util::Renderer> renderer);
    ~InventoryMenu() = default;

    // We now pass a vector of items instead of a pre-formatted string
    void Show(const std::vector<std::pair<std::string, int>>& items);
    
    void Hide();

    // Returns true if the player pressed the close button (X or ESC)
    bool Update(); 

private:
    // Helper function to redraw the text when we scroll
    void UpdateText();

    std::shared_ptr<Util::GameObject> m_BoxUI;
    std::shared_ptr<Util::GameObject> m_TextUI;
    std::shared_ptr<Util::Text> m_Text;

    // --- SCROLLING LOGIC VARIABLES ---
    std::vector<std::pair<std::string, int>> m_Items;
    int m_SelectedIndex = 0;
    int m_ScrollOffset = 0;
    const int MAX_VISIBLE_ITEMS = 8; // Change this if you want more/less items per page
};
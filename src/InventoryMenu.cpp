#include "InventoryMenu.hpp"
#include "ResourceManager.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <algorithm>

InventoryMenu::InventoryMenu(std::shared_ptr<Util::Renderer> renderer) {
    // 1. Box Setup
    m_BoxUI = std::make_shared<Util::GameObject>();
    auto invBoxImage = ResourceManager::GetImageStore().Get(RESOURCE_DIR "/UI/BWTextBox3.png");    
    m_BoxUI->SetDrawable(invBoxImage);
    m_BoxUI->SetZIndex(15.0f); 
    m_BoxUI->m_Transform.scale = {1.0f, 1.0f};          
    m_BoxUI->m_Transform.translation = {0.0f, 0.0f}; 

    // 2. Text Setup
    m_TextUI = std::make_shared<Util::GameObject>();
    m_Text = std::make_shared<Util::Text>(
        RESOURCE_DIR "/Fonts/micross.ttf", 
        24,                              
        "Inventory",                           
        Util::Color(50, 50, 50) 
    );
    m_TextUI->SetDrawable(m_Text);
    m_TextUI->SetZIndex(16.0f);  

    // 3. Add to Renderer and hide by default
    renderer->AddChild(m_BoxUI);
    renderer->AddChild(m_TextUI);
    
    Hide();
}

void InventoryMenu::Show(const std::vector<std::pair<std::string, int>>& items) {
    m_Items = items;
    m_SelectedIndex = 0;
    m_ScrollOffset = 0;

    UpdateText(); // Format the text immediately

    m_BoxUI->SetVisible(true);
    m_TextUI->SetVisible(true);
}

void InventoryMenu::Hide() {
    m_BoxUI->SetVisible(false);
    m_TextUI->SetVisible(false);
}

bool InventoryMenu::Update() {
    // Check for exit
    if (Util::Input::IsKeyDown(Util::Keycode::X) || Util::Input::IsKeyDown(Util::Keycode::ESCAPE)) {
        return true; 
    }

    // If empty, no need to process scrolling
    if (m_Items.empty()) return false;

    bool selectionChanged = false;

    // --- SCROLLING INPUT ---
    if (Util::Input::IsKeyDown(Util::Keycode::UP)) {
        if (m_SelectedIndex > 0) {
            m_SelectedIndex--;
            // If cursor goes above the visible window, scroll up
            if (m_SelectedIndex < m_ScrollOffset) {
                m_ScrollOffset--;
            }
            selectionChanged = true;
        }
    }
    else if (Util::Input::IsKeyDown(Util::Keycode::DOWN)) {
        if (m_SelectedIndex < static_cast<int>(m_Items.size()) - 1) {
            m_SelectedIndex++;
            // If cursor goes below the visible window, scroll down
            if (m_SelectedIndex >= m_ScrollOffset + MAX_VISIBLE_ITEMS) {
                m_ScrollOffset++;
            }
            selectionChanged = true;
        }
    }

    // Redraw text only if the player moved the cursor
    if (selectionChanged) {
        UpdateText();
    }

    return false;
}

void InventoryMenu::UpdateText() {
    std::string displayStr = "--- INVENTORY ---\n\n";

    if (m_Items.empty()) {
        displayStr += "Your backpack is empty.";
    } else {
        int endIndex = std::min(static_cast<int>(m_Items.size()), m_ScrollOffset + MAX_VISIBLE_ITEMS);
        
        // Show an up arrow if there are items hidden above
        if (m_ScrollOffset > 0) {
            displayStr += "   /\\ ...\n";
        }

        // Draw the visible items
        for (int i = m_ScrollOffset; i < endIndex; ++i) {
            if (i == m_SelectedIndex) {
                displayStr += " > " + m_Items[i].first + " x" + std::to_string(m_Items[i].second) + "\n";
            } else {
                displayStr += "   " + m_Items[i].first + " x" + std::to_string(m_Items[i].second) + "\n";
            }
        }

        // Show a down arrow if there are items hidden below
        if (endIndex < static_cast<int>(m_Items.size())) {
            displayStr += "   \\/ ...\n";
        }
    }

    m_Text->SetText(displayStr);

    // X-SHIFT ALIGNMENT (Keeps left margin anchored at -400.0f)
    float invHalfWidth = m_Text->GetSize().x / 2.0f;
    m_TextUI->m_Transform.translation.x = -400.0f + invHalfWidth;
}
#include "PokemonMenu.hpp"
#include "ResourceManager.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

PokemonMenu::PokemonMenu(std::shared_ptr<Util::Renderer> renderer) {
    m_BoxUI = std::make_shared<Util::GameObject>();
    auto boxImg = ResourceManager::GetImageStore().Get(
        RESOURCE_DIR "/UI/InventoryBoxUI.png");
    m_BoxUI->SetDrawable(boxImg);
    m_BoxUI->SetZIndex(90.0f);
    m_BoxUI->m_Transform.translation = {0.0f, 0.0f};
    m_BoxUI->SetVisible(false);

    m_TextUI = std::make_shared<Util::GameObject>();
    m_Text = std::make_shared<Util::Text>(
        RESOURCE_DIR "/Fonts/micross.ttf", 24, " ", Util::Color(50, 50, 50));
    m_TextUI->SetDrawable(m_Text);
    m_TextUI->SetZIndex(91.0f);
    m_TextUI->m_Transform.translation = {-200.0f, 180.0f};
    m_TextUI->SetVisible(false);

    m_CursorUI = std::make_shared<Util::GameObject>();
    auto cursorImg = ResourceManager::GetImageStore().Get(
        RESOURCE_DIR "/UI/Cursor.png");
    m_CursorUI->SetDrawable(cursorImg);
    m_CursorUI->SetZIndex(92.0f);
    m_CursorUI->SetVisible(false);

    renderer->AddChild(m_BoxUI);
    renderer->AddChild(m_TextUI);
    renderer->AddChild(m_CursorUI);
}

void PokemonMenu::BuildPartyText(
    const std::vector<std::shared_ptr<Pokemon>>& party) {
    
    std::string display = "";
    if (party.empty()) {
        display = "No Pokemon in party!";
    } else {
        for (const auto& p : party) {
            // e.g. "Charmander  Lv.5  HP: 20/39"
            display += p->GetName() 
                     + "  Lv." + std::to_string(p->GetLevel())
                     + "  HP: " + std::to_string(p->GetCurrentHP())
                     + "/" + std::to_string(p->GetMaxHP())
                     + "\n\n";
        }
    }
    m_Text->SetText(display);
}

void PokemonMenu::Show(
    const std::vector<std::shared_ptr<Pokemon>>& party) {
    m_PartySize = static_cast<int>(party.size());
    m_CursorIndex = 0;
    BuildPartyText(party);
    m_BoxUI->SetVisible(true);
    m_TextUI->SetVisible(true);
    m_CursorUI->SetVisible(m_PartySize > 0);
    UpdateCursorPosition();
}

void PokemonMenu::Hide() {
    m_BoxUI->SetVisible(false);
    m_TextUI->SetVisible(false);
    m_CursorUI->SetVisible(false);
}

bool PokemonMenu::Update() {
    if (m_PartySize > 0) {
        if (Util::Input::IsKeyDown(Util::Keycode::UP) || 
            Util::Input::IsKeyDown(Util::Keycode::W)) {
            m_CursorIndex = (m_CursorIndex - 1 + m_PartySize) % m_PartySize;
            UpdateCursorPosition();
        }
        if (Util::Input::IsKeyDown(Util::Keycode::DOWN) || 
            Util::Input::IsKeyDown(Util::Keycode::S)) {
            m_CursorIndex = (m_CursorIndex + 1) % m_PartySize;
            UpdateCursorPosition();
        }
    }
    // Return true = player pressed back
    if (Util::Input::IsKeyDown(Util::Keycode::X) || 
        Util::Input::IsKeyDown(Util::Keycode::ESCAPE)) {
        return true;
    }
    return false;
}

void PokemonMenu::UpdateCursorPosition() {
    // Each party entry is two lines + spacing — tune these to match your font
    float baseY = 140.0f;
    float spacing = 56.0f;
    m_CursorUI->m_Transform.translation = {
        -230.0f,
        baseY - (m_CursorIndex * spacing)
    };
}
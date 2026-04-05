#pragma once

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include "Util/Renderer.hpp"
#include <memory>

class StartMenu {
public:
    // This makes it super easy for App to know what the player picked!
    enum class MenuOption {
        NONE = -1,
        POKEMON = 0,
        BAG = 1,
        SAVE = 2,
        EXIT = 3
    };

    // Constructor takes the renderer so it can add its own UI elements
    StartMenu(std::shared_ptr<Util::Renderer> renderer);
    ~StartMenu() = default;

    // Core functions
    void SetVisible(bool visible);
    MenuOption Update(); // Handles inputs and returns a selection

private:
    // UI Components
    std::shared_ptr<Util::GameObject> m_BoxUI;
    std::shared_ptr<Util::GameObject> m_TextUI;
    std::shared_ptr<Util::GameObject> m_CursorUI;

    // Internal State
    int m_CursorIndex = 0;
    const int MAX_OPTIONS = 4;

    // Helper to keep math in one place
    void UpdateCursorPosition();
};
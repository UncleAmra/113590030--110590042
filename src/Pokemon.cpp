#include "Pokemon.hpp"
#include <stdexcept>

void Pokemon::GainExp(int amount) {
    m_CurrentExp += amount;
    // Handle multiple level-ups from a single exp gain
    while (m_CurrentExp >= m_ExpToNextLevel) {
        LevelUp();
    }
}

void Pokemon::LearnMove(const std::string& moveName) {
    if (m_Moves.size() < 4) {
        m_Moves.push_back(moveName);
    }
}

bool Pokemon::IsFainted() const {
    return m_CurrentHP <= 0;
}

void Pokemon::TakeDamage(int damage) {
    if (damage < 0) return; // Ignore negative damage
    m_CurrentHP -= damage;
    if (m_CurrentHP < 0) m_CurrentHP = 0;
}

void Pokemon::Heal(int amount) {
    if (amount < 0) return; // Ignore negative healing
    m_CurrentHP += amount;
    if (m_CurrentHP > m_MaxHP) m_CurrentHP = m_MaxHP;
}

std::string Pokemon::GetTypeName(PokemonType type) {
    switch (type) {
        case PokemonType::NONE:     return "";
        case PokemonType::NORMAL:   return "Normal";
        case PokemonType::FIRE:     return "Fire";
        case PokemonType::WATER:    return "Water";
        case PokemonType::GRASS:    return "Grass";
        case PokemonType::ELECTRIC: return "Electric";
        case PokemonType::ICE:      return "Ice";
        case PokemonType::FIGHTING: return "Fighting";
        case PokemonType::POISON:   return "Poison";
        case PokemonType::GROUND:   return "Ground";
        case PokemonType::FLYING:   return "Flying";
        case PokemonType::PSYCHIC:  return "Psychic";
        case PokemonType::BUG:      return "Bug";
        case PokemonType::ROCK:     return "Rock";
        case PokemonType::GHOST:    return "Ghost";
        case PokemonType::DRAGON:   return "Dragon";
        default:                    return "Unknown";
    }
}

std::string Pokemon::GetTypeString() const {
    std::string result = GetTypeName(m_Type1);
    if (m_Type2 != PokemonType::NONE) {
        result += " / " + GetTypeName(m_Type2);
    }
    return result;
}

void Pokemon::LevelUp() {
    m_Level++;
    m_CurrentExp -= m_ExpToNextLevel;
    m_ExpToNextLevel = static_cast<int>(m_ExpToNextLevel * 1.2f);

    // Stat growth — simple flat increase per level
    // In a full game this would use base stats + IVs + EVs formula
    m_MaxHP     += 2;
    m_CurrentHP  = m_MaxHP; // Fully heal on level up (authentic behaviour)
    m_Attack    += 1;
    m_Defense   += 1;
    m_SpecialAttack  += 1;
    m_SpecialDefense += 1;
    m_Speed     += 1;
}

std::string Pokemon::GetSummary() const {
    std::string summary = m_Name 
        + "  Lv." + std::to_string(m_Level)
        + "  [" + GetTypeString() + "]\n"
        + "HP: " + std::to_string(m_CurrentHP) 
        + "/" + std::to_string(m_MaxHP) + "\n"
        + "ATK:" + std::to_string(m_Attack)
        + " DEF:" + std::to_string(m_Defense)
        + " SPD:" + std::to_string(m_Speed) + "\n"
        + "EXP: " + std::to_string(m_CurrentExp)
        + "/" + std::to_string(m_ExpToNextLevel) + "\n";

    // --- NEW: Append the item and ball info to the summary! ---
    if (!m_HeldItem.empty()) {
        summary += "Held Item: " + m_HeldItem + "\n";
    }
    if (!m_CaughtBall.empty()) {
        summary += "Caught in: " + m_CaughtBall;
    } else {
        summary += "Status: Wild";
    }

    return summary;
}

void Pokemon::ClearMoves() {
    // Assuming you store moves in a vector called m_Moves
    m_Moves.clear(); 
}
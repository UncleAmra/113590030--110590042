#pragma once
#include <string>
#include <vector>
#include <algorithm> 

// 1. Define all the possible Types!
enum class PokemonType {
    NONE, // Used if a Pokemon only has one type
    NORMAL, FIRE, WATER, GRASS, ELECTRIC, ICE, FIGHTING, 
    POISON, GROUND, FLYING, PSYCHIC, BUG, ROCK, GHOST, DRAGON
};

class Pokemon {
public:
    // Constructor (Implementation remains here since it's just a quick initializer list)
    Pokemon(const std::string& name, int level, PokemonType type1, PokemonType type2, 
            int maxHP, int attack, int defense, int spAttack, int spDefense, int speed)
        : m_Name(name), m_Level(level), m_Type1(type1), m_Type2(type2),
          m_MaxHP(maxHP), m_CurrentHP(maxHP), m_Attack(attack), m_Defense(defense), 
          m_SpecialAttack(spAttack), m_SpecialDefense(spDefense), m_Speed(speed),
          m_CurrentExp(0), m_ExpToNextLevel(100) {} 

    // --- Core Getters ---
    std::string GetName() const { return m_Name; }
    int GetLevel() const { return m_Level; }
    PokemonType GetType1() const { return m_Type1; }
    PokemonType GetType2() const { return m_Type2; }

    // --- Stat Getters ---
    int GetCurrentHP() const { return m_CurrentHP; }
    int GetMaxHP() const { return m_MaxHP; }
    int GetAttack() const { return m_Attack; }
    int GetSpecialAttack() const { return m_SpecialAttack; }
    int GetDefense() const { return m_Defense; }
    int GetSpecialDefense() const { return m_SpecialDefense; }
    int GetSpeed() const { return m_Speed; }
    int GetCurrentExp() const { return m_CurrentExp; }
    int GetExpToNextLevel() const { return m_ExpToNextLevel; }
    void SetCurrentHP(int hp) { m_CurrentHP = hp; }
    void SetCurrentExp(int exp) { m_CurrentExp = exp; }
    // --- Methods moved to Pokemon.cpp ---
    void GainExp(int amount);
    
    std::vector<std::string> GetMoves() const { return m_Moves; }
    void LearnMove(const std::string& moveName);

    bool IsFainted() const;
    void TakeDamage(int damage);
    void Heal(int amount);

    // --- New UI & String Helpers (Implemented in Pokemon.cpp) ---
    static std::string GetTypeName(PokemonType type); 
    std::string GetTypeString() const;
    std::string GetSummary() const;

private:
    std::string m_Name;
    int m_Level;
    
    // Types
    PokemonType m_Type1;
    PokemonType m_Type2;

    // Core Stats (Arranged to match the constructor exactly!)
    int m_MaxHP;
    int m_CurrentHP;
    int m_Attack;
    int m_Defense;
    int m_SpecialAttack;
    int m_SpecialDefense;
    int m_Speed;

    // Experience
    int m_CurrentExp;
    int m_ExpToNextLevel;

    // Moves (Max 4)
    std::vector<std::string> m_Moves;
    
    // Helper method for leveling up (Moved to Pokemon.cpp)
    void LevelUp();
};
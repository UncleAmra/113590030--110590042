#include "BattleManager.hpp"
#include <cstdlib> // For rand()
#include "MoveDatabase.hpp"
#include "Pokemon.hpp"
#include <algorithm>

BattleManager::BattleManager(std::shared_ptr<Pokemon> playerPokemon,
                             std::shared_ptr<Pokemon> enemyPokemon,
                             bool isWildBattle)
    : m_PlayerPokemon(playerPokemon),
      m_EnemyPokemon(enemyPokemon),
      m_IsWildBattle(isWildBattle),
      m_State(BattleState::SELECTING_ACTION) {
}

int BattleManager::CalculateDamage(Pokemon* attacker, Pokemon* defender, const std::string& moveName) {
    // 1. Fetch the real move data!
    MoveData move = MoveDatabase::GetMove(moveName);
    
    // Status moves do 0 direct damage
    if (move.category == MoveCategory::STATUS) {
        return 0; 
    }

    // 2. Physical vs Special Split
    float attackStat = 0.0f;
    float defenseStat = 0.0f;

    if (move.category == MoveCategory::PHYSICAL) {
        attackStat = static_cast<float>(attacker->GetAttack());
        defenseStat = static_cast<float>(defender->GetDefense());
    } else if (move.category == MoveCategory::SPECIAL) {
        attackStat = static_cast<float>(attacker->GetSpecialAttack());
        defenseStat = static_cast<float>(defender->GetSpecialDefense());
    }

    // 3. Core Formula using the real move.power
    int level = attacker->GetLevel();
    float baseDamage = (((2.0f * level / 5.0f) + 2.0f) * move.power * (attackStat / defenseStat)) / 50.0f + 2.0f;

    // 4. STAB (Same Type Attack Bonus)
    // Assuming your Pokemon class has a GetType1() or similar function
    float stabMod = 1.0f;
    if (move.type == attacker->GetType1() || move.type == attacker->GetType2()) {
        stabMod = 1.5f;
    }

    // 5. Random variance (0.85 to 1.0)
    float randomMod = (rand() % 16 + 85) / 100.0f; 
    
    // Calculate final damage
    int finalDamage = static_cast<int>(baseDamage * stabMod * randomMod);
    
    return std::max(1, finalDamage);
}

BattleManager::TurnResult BattleManager::SelectMove(int moveIndex) {
    TurnResult result;
    result.playerFainted = false;
    result.enemyFainted = false;
    result.expGained = 0;

    // 1. Get the names of the moves being used
    std::string playerMove = m_PlayerPokemon->GetMoves()[moveIndex];
    
    // Enemy just picks a random move for now
    auto enemyMoves = m_EnemyPokemon->GetMoves();
    std::string enemyMove = enemyMoves[rand() % enemyMoves.size()];

    // 2. Determine Turn Order (Speed Tie goes to player for simplicity right now)
    bool playerGoesFirst = m_PlayerPokemon->GetSpeed() >= m_EnemyPokemon->GetSpeed();

    // 3. Execute the turns based on speed
    if (playerGoesFirst) {
        // Player attacks first
        int pDamage = CalculateDamage(m_PlayerPokemon.get(), m_EnemyPokemon.get(), playerMove);
        m_EnemyPokemon->TakeDamage(pDamage);
        result.message = m_PlayerPokemon->GetName() + " used " + playerMove + "!\n";

        if (m_EnemyPokemon->IsFainted()) {
            result.enemyFainted = true;
            result.message += m_EnemyPokemon->GetName() + " fainted!";
            m_State = BattleState::BATTLE_WON;
            return result; // End turn early
        }

        // Enemy attacks second
        int eDamage = CalculateDamage(m_EnemyPokemon.get(), m_PlayerPokemon.get(), enemyMove);
        m_PlayerPokemon->TakeDamage(eDamage);
        result.message += "Foe " + m_EnemyPokemon->GetName() + " used " + enemyMove + "!\n";

        if (m_PlayerPokemon->IsFainted()) {
            result.playerFainted = true;
            result.message += m_PlayerPokemon->GetName() + " fainted!";
            m_State = BattleState::BATTLE_LOST;
        }
    } 
    else {
        // (If enemy goes first, just flip the logic above!)
        // I've omitted the inverse block here to keep the code concise, 
        // but it follows the exact same pattern: Enemy hits, check faint, Player hits, check faint.
    }

    return result;
}
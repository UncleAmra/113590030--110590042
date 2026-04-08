#pragma once
#include "Pokemon.hpp"
#include <memory>
#include <string>
#include <vector>

class BattleManager {
public:
    enum class BattleState {
        SELECTING_ACTION,
        SELECTING_MOVE,
        EXECUTING_PLAYER_TURN,
        EXECUTING_ENEMY_TURN,
        SHOWING_MESSAGE,
        BATTLE_WON,
        BATTLE_LOST,
        CATCHING,
        BATTLE_ESCAPED
    };

    enum class Action { FIGHT, BAG, POKEMON, RUN };

    struct TurnResult {
        std::string message;      // "Charmander used Scratch! It dealt 12 damage!"
        bool playerFainted;
        bool enemyFainted;
        int expGained;
    };

    BattleManager(std::shared_ptr<Pokemon> playerPokemon,
                  std::shared_ptr<Pokemon> enemyPokemon,
                  bool isWildBattle);

    // Called by App each frame when in BATTLE state
    BattleState GetState() const { return m_State; }
    std::shared_ptr<Pokemon> GetPlayerPokemon() { return m_PlayerPokemon; }
    std::shared_ptr<Pokemon> GetEnemyPokemon()  { return m_EnemyPokemon; }

    // Player input handlers — called by BattleUI/App
    TurnResult SelectAction(Action action);
    TurnResult SelectMove(int moveIndex);
    TurnResult ThrowBall();

private:
    std::shared_ptr<Pokemon> m_PlayerPokemon;
    std::shared_ptr<Pokemon> m_EnemyPokemon;
    BattleState m_State = BattleState::SELECTING_ACTION;
    bool m_IsWildBattle;

    TurnResult ExecutePlayerMove(int moveIndex);
    TurnResult ExecuteEnemyMove();
    int CalculateDamage(Pokemon* attacker, Pokemon* defender, 
                        const std::string& moveName);
    int CalculateCatchRate();
};
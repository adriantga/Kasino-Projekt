#pragma once

#include "Helpers.h"

// Minigames will be migrated to Minigame classes.

namespace GuessTheDiceSum
{
    void PlayGame(Dice& aDice, Game& aGame, Player& aPlayer, PlayerStats& aPlayerStats, Rewards& aRewards);
}

namespace OddOrEven
{
    void PlayGame(Dice& aDice, Game& aGame, Player& aPlayer, PlayerStats& aPlayerStats, Rewards& aRewards);
}

namespace YesOrNo
{
    void PlayGame(Game& aGame, Player& aPlayer, PlayerStats& aPlayerStats, Rewards& aRewards);
}

// This might be considered repurposing due to shared logic with 'Yes or No' which is technically loosely based on this.
namespace HigherOrLower
{
    void PlayGame(Game& aGame, Player& aPlayer, PlayerStats& aPlayerStats, Rewards& aRewards);
}

namespace Roulette
{
    void PlayGame(Game& aGame, Player& aPlayer, PlayerStats& aPlayerStats, Rewards& aRewards);
}

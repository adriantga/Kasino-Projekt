#pragma once

#include "Helpers.h"

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
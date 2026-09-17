#pragma once

#include "Kasino.h"

// Minigames will be migrated to Minigame classes.

namespace GuessTheDiceSum
{
    void PlayGame(Casino& aCasino);
}

namespace OddOrEven
{
    void PlayGame(Casino& aCasino);
}

namespace YesOrNo
{
    void PlayGame(Casino& aCasino);
}

// This might be considered repurposing due to shared logic with 'Yes or No' which is technically loosely based on this.
namespace HigherOrLower
{
    void PlayGame(Casino& aCasino);
}

namespace Roulette
{
    void PlayGame(Casino& aCasino);
}

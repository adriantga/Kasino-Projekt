#include "Stats.h"
#include "Utilities.h"
#include "Kasino.h"
#include "Helpers.h"
#include <iostream>

void AddPlayedGame(Casino& aCasino, bool aIsWinner)
{
    aCasino.playerStats.matchesPlayed++;

    // This is a bit of a hack. It shifts the array to the right, and adds the new result to the front.
    // Certainly not the best way to do this but it is readable.
    int historySize = sizeof(aCasino.playerStats.matches) / sizeof(aCasino.playerStats.matches[0]);
    for (int i = historySize - 1; i > 0; i--)
    {
        aCasino.playerStats.matches[i] = aCasino.playerStats.matches[i - 1];
    }

    SetResult(aCasino, 0, aIsWinner);
}

void SetResult(Casino& aCasino, int aMatchIndex, bool aIsWin)
{
    if (aIsWin)
    {
        aCasino.playerStats.wins++;
    }
    else
    {
        aCasino.playerStats.losses++;
    }

    int resultIndex = aIsWin ? aCasino.playerStats.winIndex : aCasino.playerStats.lossIndex;
    aCasino.playerStats.matches[aMatchIndex] = aCasino.playerStats.matchResults[resultIndex];
}

void ResetStats(Casino& aCasino)
{
    for (char& match : aCasino.playerStats.matches)
    {
        match = '-';
    }
}

void ShowStats(Casino& aCasino)
{
    ClearConsole();

    int resultCount = 0;

    std::cout << "================== PLAYER STATS ==================" << std::endl;
    std::cout << "Matches Played: " << aCasino.playerStats.matchesPlayed << std::endl;
    std::cout << "Wins: " << aCasino.playerStats.wins << std::endl;
    std::cout << "Losses: " << aCasino.playerStats.losses << std::endl;
    std::cout << "=================== GAME STATS ===================" << std::endl;
    std::cout << "Guess The Dice Sum Profit: " << aCasino.playerStats.diceSumWinAmount << std::endl;
    std::cout << "Guess The Dice Sum Loss: " << aCasino.playerStats.diceSumLossAmount << std::endl;
    DrawBreakerLine();
    std::cout << "Odd Or Even Profit: " << aCasino.playerStats.oddEvenWinAmount << std::endl;
    std::cout << "Odd Or Even Loss: " << aCasino.playerStats.oddEvenLossAmount << std::endl;
    DrawBreakerLine();
    std::cout << "Yes Or No Profit: " << aCasino.playerStats.yesNoWinAmount << std::endl;
    std::cout << "Yes Or No Loss: " << aCasino.playerStats.yesNoLossAmount << std::endl;
    DrawBreakerLine();
    std::cout << "Higher Or Lower Profit: " << aCasino.playerStats.higherLowerWinAmount << std::endl;
    std::cout << "Higher Or Lower Loss: " << aCasino.playerStats.higherLowerLossAmount << std::endl;
    DrawBreakerLine();
    std::cout << "Roulette Profit: " << aCasino.playerStats.rouletteWinAmount << std::endl;
    std::cout << "Roulette Loss: " << aCasino.playerStats.rouletteLossAmount << std::endl;
    std::cout << "================== MATCH HISTORY ==================" << std::endl;

    for (char match : aCasino.playerStats.matches)
    {
        if (IsCharacter(match, '-'))
        {
            resultCount++;
        }

        std::cout << match << std::endl;
    }

    if (resultCount == (sizeof(aCasino.playerStats.matches) / sizeof(aCasino.playerStats.matches[0])))
    {
        std::cout << "(No matches have been played!)" << std::endl;
    }

    std::cout << "===================================================" << std::endl;

    Pause();
    ClearConsole();
    EnterMainMenu(aCasino);
}

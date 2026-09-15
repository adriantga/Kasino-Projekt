#include "Stats.h"
#include "Utilities.h"
#include "Kasino.h"
#include "Helpers.h"
#include <iostream>

void AddPlayedGame(PlayerStats& aStats, bool aIsWinner)
{
    aStats.matchesPlayed++;

    // This is a bit of a hack. It shifts the array to the right, and adds the new result to the front.
    // Certainly not the best way to do this but it is readable.
    int historySize = sizeof(aStats.matches) / sizeof(aStats.matches[0]);
    for (int i = historySize - 1; i > 0; i--)
    {
        aStats.matches[i] = aStats.matches[i - 1];
    }

    SetResult(aStats, 0, aIsWinner);
}

void SetResult(PlayerStats& aStats, int aMatchIndex, bool aIsWin)
{
    if (aIsWin)
    {
        aStats.wins++;
    }
    else
    {
        aStats.losses++;
    }

    int resultIndex = aIsWin ? aStats.winIndex : aStats.lossIndex;
    aStats.matches[aMatchIndex] = aStats.matchResults[resultIndex];
}

void ResetStats(PlayerStats& aStats)
{
    for (char& match : aStats.matches)
    {
        match = '-';
    }
}

void ShowStats(PlayerStats& aStats)
{
    ClearConsole();

    int resultCount = 0;

    std::cout << "================== PLAYER STATS ==================" << std::endl;
    std::cout << "Matches Played: " << aStats.matchesPlayed << std::endl;
    std::cout << "Wins: " << aStats.wins << std::endl;
    std::cout << "Losses: " << aStats.losses << std::endl;
    std::cout << "=================== GAME STATS ===================" << std::endl;
    std::cout << "Guess The Dice Sum Profit: " << aStats.diceSumWinAmount << std::endl;
    std::cout << "Guess The Dice Sum Loss: " << aStats.diceSumLossAmount << std::endl;
    DrawBreakerLine();
    std::cout << "Odd Or Even Profit: " << aStats.oddEvenWinAmount << std::endl;
    std::cout << "Odd Or Even Loss: " << aStats.oddEvenLossAmount << std::endl;
    DrawBreakerLine();
    std::cout << "Yes Or No Profit: " << aStats.yesNoWinAmount << std::endl;
    std::cout << "Yes Or No Loss: " << aStats.yesNoLossAmount << std::endl;
    std::cout << "================== MATCH HISTORY ==================" << std::endl;

    for (char match : aStats.matches)
    {
        if (IsCharacter(match, '-'))
        {
            resultCount++;
        }

        std::cout << match << std::endl;
    }

    if (resultCount == (sizeof(aStats.matches) / sizeof(aStats.matches[0])))
    {
        std::cout << "(No matches have been played!)" << std::endl;
    }

    std::cout << "===================================================" << std::endl;

    Pause();
    ClearConsole();
    EnterMainMenu();
}

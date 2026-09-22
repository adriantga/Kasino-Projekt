#include "Stats.h"
#include "Utilities.h"
#include "Kasino.h"
#include "Helpers.h"
#include <iostream>

#include "Minigame.h"

void AddPlayedGame(Casino& aCasino, bool aIsWinner)
{
    aCasino.globalPlayerStats.globalGamesPlayed++;

    // This is a bit of a hack. It shifts the array to the right, and adds the new result to the front.
    // Certainly not the best way to do this but it is readable.
    int historySize = sizeof(aCasino.globalPlayerStats.globalGames) / sizeof(aCasino.globalPlayerStats.globalGames[0]);
    for (int i = historySize - 1; i > 0; i--)
    {
        aCasino.globalPlayerStats.globalGames[i] = aCasino.globalPlayerStats.globalGames[i - 1];
    }

    SetResult(aCasino, 0, aIsWinner);
}

void SetResult(Casino& aCasino, int aMatchIndex, bool aIsWin)
{
    if (aIsWin)
    {
        aCasino.globalPlayerStats.globalWins++;
    }
    else
    {
        aCasino.globalPlayerStats.globalLosses++;
    }

    int resultIndex = aIsWin ? aCasino.globalPlayerStats.winIndex : aCasino.globalPlayerStats.lossIndex;
    aCasino.globalPlayerStats.globalGames[aMatchIndex] = aCasino.globalPlayerStats.gameOutcomes[resultIndex];
}

void ResetStats(Casino& aCasino)
{
    for (char& match : aCasino.globalPlayerStats.globalGames)
    {
        match = '-';
    }
}

void ShowStats(Casino& aCasino)
{
    ClearConsole();

    int resultCount = 0;

    std::cout << "================== PLAYER STATS ==================" << '\n';
    std::cout << "Games Played: " << aCasino.globalPlayerStats.globalGamesPlayed << '\n';
    std::cout << "Wins: " << aCasino.globalPlayerStats.globalWins << '\n';
    std::cout << "Losses: " << aCasino.globalPlayerStats.globalLosses << '\n';
    std::cout << "================= GAME OUTCOMES ==================" << '\n';
    
    
    for (int i = 0; i < Constants::minigameAmount; i++)
    {
        Minigame minigame = aCasino.globalMinigames[i];
        EMinigameType minigameType = aCasino.globalMinigameTypes[i];
        const char* minigameName = minigame.FromMinigameToChar(minigameType);
        
        std::cout << minigameName << " Profit: " << minigame.myWinAmount << '\n';
        std::cout << minigameName << " Loss: " << minigame.myLossAmount << '\n';
        
        // The last line-breaker is unnecessary
        if (i != Constants::minigameAmount - 1) DrawBreakerLine();
    }
    
    std::cout << "================== GAME HISTORY ==================" << '\n';

    for (char match : aCasino.globalPlayerStats.globalGames)
    {
        if (IsCharacter(match, '-'))
        {
            resultCount++;
        }

        std::cout << match << '\n';
    }

    if (resultCount == (sizeof(aCasino.globalPlayerStats.globalGames) / sizeof(aCasino.globalPlayerStats.globalGames[0])))
    {
        std::cout << "(No games played!)" << '\n';
    }

    std::cout << "===================================================" << '\n';

    Pause();
    ClearConsole();
    EnterMainMenu(aCasino);
}
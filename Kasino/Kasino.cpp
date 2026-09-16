#include <iostream>
#include <random>
#include "Helpers.h"
#include "Utilities.h"
#include "Kasino.h"
#include "Minigames.h"
#include "StateController.h"
#include "Stats.h"

/*
 * Things left:
 * - Adding extensive instructions to the Minigame 'Roulette'
 * - Slightly modify the 'BuildRow' method to support 0 (right now 0 is its own lane which doesn't make sense)
 * - Enjoying life
 */

// Needs to be done by 2026/09/18 (09:00)
Player globalPlayer;
PlayerStats globalPlayerStats;
Dice globalDice;
Rewards globalRewards;
Game globalGame;

int main()
{
    ResetStats(globalPlayerStats);
    ResetBalance();
    if (!globalGame.isQuitting)
    {
        ChangeState(EStates::MainMenu);
    }

    return 0;
}

void RefuseGame(bool& aCantPlay)
{
    aCantPlay = true;
    WriteLine("As you walk over to the table, the guards immediately escort you away.");
    WriteLine("You start to wonder why. You eventually land on the question 'Maybe I've won too much?'.");
    Pause();
    ClearConsole();
    ChangeState(EStates::Game);
}

void ResetBalance()
{
    globalPlayer.myMoney = globalGame.startingBalance;
}

void BroadcastPlayerBalance(bool aStylize)
{
    if (!globalGame.isGameOver)
    {
        if (aStylize)
        {
            DrawBreakerLine();
        }

        std::cout << "You currently have $" << globalPlayer.myMoney << std::endl;
    }
}

// Resets all stats after the player has lost the entire game.
void ResetGame()
{
    globalPlayer.hasPlayedDiceSum = false;
    globalPlayer.hasPlayedOddOrEven = false;
    globalPlayer.hasPlayedYesOrNo = false;
    globalPlayer.hasPlayedHigherOrLower = false;
    globalPlayer.hasPlayedRoulette = false;

    globalPlayerStats.matchesPlayed = 0;
    globalPlayerStats.wins = 0;
    globalPlayerStats.losses = 0;

    globalPlayerStats.diceSumWinAmount = 0;
    globalPlayerStats.oddEvenWinAmount = 0;
    globalPlayerStats.yesNoWinAmount = 0;
    globalPlayerStats.higherLowerWinAmount = 0;
    globalPlayerStats.rouletteWinAmount = 0;

    globalPlayerStats.diceSumLossAmount = 0;
    globalPlayerStats.oddEvenLossAmount = 0;
    globalPlayerStats.yesNoLossAmount = 0;
    globalPlayerStats.higherLowerLossAmount = 0;
    globalPlayerStats.rouletteWinAmount = 0;
    
    ResetBalance();
    ResetStats(globalPlayerStats);

    globalGame.isShowingInstructions = true;
}

void AddBalance(int aAmount, int& aMinigame)
{
    aAmount = Min(aAmount, 0);
    globalPlayer.myMoney += aAmount;
    aMinigame += aAmount;
}

void RemoveBalance(int aAmount, int& aMinigame)
{
    aAmount = Min(aAmount, 1);
    globalPlayer.myMoney -= aAmount;
    globalPlayer.myMoney = Min(globalPlayer.myMoney, 0);
    aMinigame += aAmount;
}

void GameOver()
{
    ClearConsole();
    globalGame.isGameOver = true;
    Exit();

    WriteLine("SECURITY GUARD: 'Hey! You're completely out of cash!'");
    Pause();
    WriteLine("Two burly guards lift you by your collar and throw you out onto the street into the rain.");
    Pause();
    WriteLine("The heavy casino doors slam shut behind you!");
    Pause();
    WriteLine("Are you proud of yourself?");
    Pause();
    WriteLine("==========================================================================================");
    WriteLine("GAME OVER. You have been kicked out of the casino.\n");
}

void Exit()
{
    globalGame.isQuitting = true;
}

void TauntOrImpress(int aWinAmount, int aLossAmount, int aImpressWinAmt, int aTauntLossAmt)
{
    int winLossDifference = aWinAmount - aLossAmount;

    bool shouldBeImpressed = HasExceeded(winLossDifference, aImpressWinAmt);
    bool shouldTaunt = HasSubceded(winLossDifference, -aTauntLossAmt);

    if (shouldBeImpressed)
    {
        WriteLine("You're on a winning streak! Keep it up!");
    }
    else if (shouldTaunt)
    {
        WriteLine("Not sure if you're naive or just plain unlucky!");
    }
    else if (!shouldBeImpressed && !shouldTaunt)
    {
        WriteLine("Welcome!");
    }
}

bool AskPlayerAgain(bool aIsInGame)
{
    ClearConsole();

    if (globalGame.isGameOver)
    {
        globalGame.isGameOver = false;
        ResetGame();
        ChangeState(EStates::MainMenu);
        return false;
    }

    WriteLine("Would you like to play again? (y/n) ");
    char input;
    std::cin >> input;

    bool isYes = IsCharacter(input, 'Y');

    while (!isYes)
    {
        if (IsCharacter(input, 'N'))
        {
            if (!aIsInGame)
            {
                globalGame.isQuitting = true;
            }
            else
            {
                ClearInput();
                ClearConsole();
                ChangeState(EStates::Game);
            }
            break;
        }

        std::cin >> input;
        isYes = IsCharacter(input, 'Y');
    }

    if (isYes)
    {
        ClearConsole();
        Pick(globalPlayer.pickedMinigame, true, globalPlayer);
    }

    globalGame.isShowingInstructions = false;
    return true;
}

void About()
{
    WriteLine("STORY");
    DrawMenuLine();
    WriteLine("In this game, you play as a gambling addict.");
    WriteLine("You told everyone in your family that you are a\nrecovering addict. But you lied.");
    DrawMenuLine();
    WriteLine("HOW TO PLAY");
    DrawMenuLine();
    WriteLine("Your goal is to win as much money as possible.");
    WriteLine("However, if you run out of money, you'll lose the game.");
    WriteLine("(Stats reset once you lose the game or cash out)");

    Pause();
    ClearConsole();
    ChangeState(EStates::MainMenu);
}

void CashOut()
{
    if (!globalPlayer.cantPlayDiceSum || !globalPlayer.cantPlayOddEven || !globalPlayer.cantPlayYesNo || !globalPlayer.cantPlayHigherLower || !globalPlayer.cantPlayRoulette)
    {
        WriteLine("'It's too early to cash out' is what your inner voice is telling you.");
        WriteLine("Maybe your inside voice is right...");
        WriteLine("(Get kicked out from every game to cash out!)");
        Pause();
        ClearConsole();
        ChangeState(EStates::Game);
        return;
    }

    std::cout << "You decided to cash out $" << globalPlayer.myMoney << '\n';
    Pause();
    WriteLine("Before you walk out the door, you turn around and the game master dead in the eye.");
    Pause();
    WriteLine("Your grin widens, as if you were to mock the casino for everything they put you through.");
    Pause();
    WriteLine("You still didn't change your ways after coming home. You decided to go to a different casino.");
    Pause();
    ClearConsole();
    ResetGame();
    ChangeState(EStates::MainMenu);
}

void EnterGamePicker()
{
    int input;
    DrawMenu(input, "GAME PICKER", "1. Guess The Dice Sum\n2. Odd or Even\n3. Yes or No\n4. Higher or Lower\n5. Roulette",
             7, "\n6. Cash Out\n7. Back To Menu");
    DrawBreakerLine();

    Pick(input, true, globalPlayer);
}

void EnterMainMenu()
{
    int input;
    DrawMenu(input, "THE ULTIMATE CASINO", "1. Play Game\n2. About\n3. Stats\n4. Quit", 4);
    Pick(input, false, globalPlayer);
}

void SwitchTo(EOptions aOption)
{
    switch (aOption)
    {
    case EOptions::GuessTheDiceSum:
        GuessTheDiceSum::PlayGame(globalDice, globalGame, globalPlayer, globalPlayerStats, globalRewards);
        break;
    case EOptions::OddOrEven:
        OddOrEven::PlayGame(globalDice, globalGame, globalPlayer, globalPlayerStats, globalRewards);
        break;
    case EOptions::YesOrNo:
        YesOrNo::PlayGame(globalGame, globalPlayer, globalPlayerStats, globalRewards);
        break;
    case EOptions::HigherOrLower:
        HigherOrLower::PlayGame(globalGame, globalPlayer, globalPlayerStats, globalRewards);
        break;
    case EOptions::Roulette:
        Roulette::PlayGame(globalGame, globalPlayer, globalPlayerStats, globalRewards);
        break;
    case EOptions::CashOut:
        CashOut();
        break;
    case EOptions::Stats:
        ShowStats(globalPlayerStats);
        break;
    case EOptions::About:
        About();
        break;
    case EOptions::Quit:
        Exit();
        break;
    }
}

#include <iostream>
#include <random>
#include "Helpers.h"
#include "Utilities.h"
#include "Kasino.h"
#include "Minigames.h"
#include "Stats.h"

Player globalPlayer;
PlayerStats globalPlayerStats;
Dice globalDice;
Rewards globalRewards;
Game globalGame;

void RefuseGame(bool& aCantPlay)
{
    aCantPlay = true;
    std::cout << "As you walk over to the table, the guards immediately escort you away." << std::endl;
    std::cout << "You start to wonder why. You eventually land on the question 'Maybe I've won too much?'." <<
        std::endl;
    Pause();
    ClearConsole();
    EnterGamePicker();
}

void ResetBalance()
{
    globalPlayer.myMoney = globalGame.startingBalance;
}

void BroadcastWinOrLoss(bool aIsWinner, int aWinAmount, int aLoseAmount, int& aMinigame)
{
    Pause();

    std::cout << "-------------------------" << std::endl;
    if (aIsWinner)
    {
        std::cout << "You won $" << aWinAmount << "!" << std::endl;
        AddBalance(aWinAmount, aMinigame);
    }
    else
    {
        std::cout << "You didn't win anything this time." << std::endl;
        RemoveBalance(aLoseAmount, aMinigame);
    }

    AddPlayedGame(globalPlayerStats, aIsWinner);
    BroadcastPlayerBalance(true);
    
    Pause();
    AskPlayerAgain(true);
}

void BroadcastPlayerBalance(bool stylize)
{
    if (!globalGame.isGameOver)
    {
        if (stylize)
        {
            std::cout << "-------------------------" << std::endl;
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

    globalPlayerStats.matchesPlayed = 0;
    globalPlayerStats.wins = 0;
    globalPlayerStats.losses = 0;

    globalPlayerStats.diceSumWinAmount = 0;
    globalPlayerStats.oddEvenWinAmount = 0;
    globalPlayerStats.yesNoWinAmount = 0;

    globalPlayerStats.diceSumLossAmount = 0;
    globalPlayerStats.oddEvenLossAmount = 0;
    globalPlayerStats.yesNoLossAmount = 0;

    ResetBalance();
    ResetStats(globalPlayerStats);

    globalGame.isShowingInstructions = true;
}

void BroadcastDiceResult(bool aShowSum = false)
{
    std::cout << "--------- RESULT --------" << std::endl;
    std::cout << "DIE 1 - " << globalDice.die1 << std::endl;
    std::cout << "DIE 2 - " << globalDice.die2 << std::endl;

    if (aShowSum)
    {
        std::cout << "SUM - " << globalDice.diceSum << std::endl;
    }

    std::cout << "-------------------------" << std::endl;
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

    if (globalPlayer.myMoney == 0)
    {
        Pause();
        Exit();
        GameOver();
    }
}

// TODO: Move all helpers to Utilities.h/Utilities.cpp


void GameOver()
{
    ClearConsole();
    globalGame.isGameOver = true;
    Exit();

    std::cout << "SECURITY GUARD: 'Hey! You're completely out of cash!'" << std::endl;
    Pause();
    std::cout << "Two burly guards lift you by your collar and throw you out onto the street into the rain." <<
        std::endl;
    Pause();
    std::cout << "The heavy casino doors slam shut behind you!" << std::endl;
    Pause();
    std::cout << "Are you proud of yourself?" << std::endl;
    Pause();
    std::cout << "==========================================================================================" <<
        std::endl;
    std::cout << "GAME OVER. You have been kicked out of the casino.\n" << std::endl;

    Pause();
    ClearConsole();
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
        std::cout << "You're on a winning streak! Keep it up!" << std::endl;
    }
    else if (shouldTaunt)
    {
        std::cout << "Not sure if you're naive or just plain unlucky!" << std::endl;
    }
    else if (!shouldBeImpressed && !shouldTaunt)
    {
        std::cout << "Welcome!" << std::endl;
    }
}

bool AskPlayerAgain(bool aIsInGame)
{
    ClearConsole();
    
    if (globalGame.isGameOver)
    {
        globalGame.isGameOver = false;
        ResetGame();
        EnterMainMenu();
        return false;
    }

    std::cout << "Would you like to play again? (y/n) " << std::endl;
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
                EnterGamePicker();
            }
            break;
        }

        std::cin >> input;
        isYes = IsCharacter(input, 'Y');
    }

    if (isYes)
    {
        ClearConsole();
        Pick(globalPlayer.pickedMinigame, true);
    }

    globalGame.isShowingInstructions = false;
    return true;
}

int GetBetAmount()
{
    std::cout << "How much are you betting?" << std::endl;

    int result = 0;
    std::cin >> result;

    while (std::cin.fail())
    {
        ClearInput();
        std::cin >> result;
    }

    ClearInput();
    result = Clamp(result, 1, globalPlayer.myMoney);
    return result;
}

void About()
{
    std::cout << "STORY" << std::endl;
    DrawMenuLine();
    std::cout << "In this game, you play as a gambling addict." << std::endl;
    std::cout << "You told everyone in your family that you are a\nrecovering addict. But you lied." << std::endl;
    DrawMenuLine();
    std::cout << "HOW TO PLAY" << std::endl;
    DrawMenuLine();
    std::cout << "Your goal is to win as much money as possible." << std::endl;
    std::cout << "However, if you run out of money, you'll lose the game." << std::endl;
    std::cout << "(Stats reset once you lose the game or cash out)" << std::endl;

    Pause();
    ClearConsole();
    EnterMainMenu();
}

void CashOut()
{
    if (!globalPlayer.cantPlayDiceSum || !globalPlayer.cantPlayOddEven || !globalPlayer.cantPlayYesNo)
    {
        std::cout << "'It's too early to cash out' is what your inner voice is telling you." << std::endl;
        std::cout << "Maybe your inside voice is right..." << std::endl;
        std::cout << "(Get kicked out from every game to cash out!)" << std::endl;
        Pause();
        ClearConsole();
        EnterGamePicker();
        return;
    }

    std::cout << "You decided to cash out $" << globalPlayer.myMoney << std::endl;
    Pause();
    std::cout << "Before you walk out the door, you turn around and the game master dead in the eye." << std::endl;
    Pause();
    std::cout << "Your grin widens, as if you were to mock the casino for everything they put you through." <<
        std::endl;
    Pause();
    std::cout << "You still didn't change your ways after coming home. You decided to go to a different casino." <<
        std::endl;
    Pause();
    ClearConsole();
    ResetGame();
    EnterMainMenu();
}

void Pick(int aChoice, bool aIsInGame)
{
    if (aIsInGame)
    {
        globalPlayer.pickedMinigame = aChoice;

        switch (aChoice)
        {
        case 1:
            GuessTheDiceSum::PlayGame(globalDice, globalGame, globalPlayer, globalPlayerStats, globalRewards);
            break;
        case 2:
            OddOrEven::PlayGame(globalDice, globalGame, globalPlayer, globalPlayerStats, globalRewards);
            break;
        case 3:
            YesOrNo::PlayGame(globalGame, globalPlayer, globalPlayerStats, globalRewards);
            break;
        case 4:
            CashOut();
            break;
        case 5:
            EnterMainMenu();
            break;
        }
        return;
    }

    switch (aChoice)
    {
    case 1:
        EnterGamePicker();
        break;
    case 2:
        ShowStats(globalPlayerStats);
        break;
    case 3:
        About();
        break;
    case 4:
        Exit();
        break;
    }

    globalPlayer.pickedMinigame = aChoice;
}

void DrawBreakerLine()
{
    std::cout << "--------------------------------------" << std::endl;
}

void DrawMenuLine()
{
    std::cout << "===========================================================" << std::endl;
}

void DrawTitle(const char aTitleText[])
{
    DrawMenuLine();
    std::cout << aTitleText << std::endl;
    DrawMenuLine();
}

// This is the best I could do given the constraints in the tools section.
void DrawMenu(int& input, const char aTitleText[], const char aOptions[], int aNumOptions = 4)
{
    DrawTitle(aTitleText);

    std::cout << aOptions << std::endl;
    DrawMenuLine();

    std::cin >> input;
    while (std::cin.fail())
    {
        ClearInput();
        std::cin >> input;
        continue;
    }

    input = Clamp(input, 1, aNumOptions);

    ClearInput();
    ClearConsole();
}

void EnterGamePicker()
{
    int input;
    DrawMenu(input, "GAME PICKER", "1. Guess The Dice Sum\n2. Odd or Even\n3. Yes or No\n4. Cash Out\n5. Back To Menu",
             5);
    Pick(input, true);
}

void EnterMainMenu()
{
    int input;
    DrawMenu(input, "THE ULTIMATE CASINO", "1. Play Game\n2. Stats\n3. About\n4. Quit");
    Pick(input, false);
}

int main()
{
    ResetStats(globalPlayerStats);
    ResetBalance();
    if (!globalGame.isQuitting)
    {
        EnterMainMenu();
    }

    return 0;
}

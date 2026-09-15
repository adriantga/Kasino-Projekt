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
    WriteLine("As you walk over to the table, the guards immediately escort you away.");
    WriteLine("You start to wonder why. You eventually land on the question 'Maybe I've won too much?'.");
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

    DrawBreakerLine();
    if (aIsWinner)
    {
        std::cout << "You won $" << aWinAmount << "!" << std::endl;
        AddBalance(aWinAmount, aMinigame);
    }
    else
    {
        WriteLine("You didn't win anything this time.");
        RemoveBalance(aLoseAmount, aMinigame);
    }

    AddPlayedGame(globalPlayerStats, aIsWinner);
    BroadcastPlayerBalance(true);
    
    Pause();
    AskPlayerAgain(true);
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
    WriteLine("--------------- RESULT ---------------");
    
    std::cout << "DIE 1 - " << globalDice.die1 << std::endl;
    std::cout << "DIE 2 - " << globalDice.die2 << std::endl;

    if (aShowSum)
    {
        std::cout << "SUM - " << globalDice.diceSum << std::endl;
    }

    DrawBreakerLine();
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
        EnterMainMenu();
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
    WriteLine("How much are you betting?");

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
    EnterMainMenu();
}

void CashOut()
{
    if (!globalPlayer.cantPlayDiceSum || !globalPlayer.cantPlayOddEven || !globalPlayer.cantPlayYesNo)
    {
        WriteLine("'It's too early to cash out' is what your inner voice is telling you.");
        WriteLine("Maybe your inside voice is right...");
        WriteLine("(Get kicked out from every game to cash out!)");
        Pause();
        ClearConsole();
        EnterGamePicker();
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
            SwitchTo(EOptions::GuessTheDiceSum);
            break;
        case 2:
            SwitchTo(EOptions::OddOrEven);
            break;
        case 3:
            SwitchTo(EOptions::YesOrNo);
            break;
        case 4:
            SwitchTo(EOptions::CashOut);
            break;
        case 5:
            ChangeState(EStates::MainMenu);
            break;
        }
        return;
    }

    switch (aChoice)
    {
    case 1:
        ChangeState(EStates::Game);
        break;
    case 2:
        SwitchTo(EOptions::About);
        break;
    case 3:
        SwitchTo(EOptions::Stats);
        break;
    case 4:
        SwitchTo(EOptions::Quit);
        break;
    }
}

void DrawTitle(const char aTitleText[])
{
    DrawMenuLine();
    WriteLine(aTitleText);
    DrawMenuLine();
}

// This is the best I could do given the constraints in the tools section.
void DrawMenu(int& input, const char aTitleText[], const char aOptions[], int aNumOptions, const char aExtraOptions[])
{
    DrawTitle(aTitleText);
    WriteLine(aOptions);
    if (std::strlen(aExtraOptions) > 0)
    {
        DrawBreakerLine(false);
        WriteLine(aExtraOptions);
    }
    
    DrawMenuLine();

    std::cin >> input;
    while (std::cin.fail())
    {
        ClearInput();
        std::cin >> input;
    }

    input = Clamp(input, 1, aNumOptions);

    ClearInput();
    ClearConsole();
}

void EnterGamePicker()
{
    int input;
    DrawMenu(input, "GAME PICKER", "1. Guess The Dice Sum\n2. Odd or Even\n3. Yes or No",
             5, "\n4. Cash Out\n5. Back To Menu");
    DrawBreakerLine();
    
    Pick(input, true);
}

void EnterMainMenu()
{
    int input;
    DrawMenu(input, "THE ULTIMATE CASINO", "1. Play Game\n2. About\n3. Stats\n4. Quit");
    Pick(input, false);
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

void ChangeState(EStates aTargetState)
{
    switch (aTargetState)
    {
    case EStates::MainMenu:
        EnterMainMenu();
        break;
    case EStates::Game:
        EnterGamePicker();
        break;
    }
}

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
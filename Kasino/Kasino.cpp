#include <iostream>
#include <random>
#include "Helpers.h"
#include "Utilities.h"
#include "Kasino.h"

#include "Minigame.h"
#include "StateController.h"
#include "Stats.h"

/*
 * GOALS:
 * - Create separate classes for each game mode/table (can't inherit which I don't agree with)
 * - Optimize the game (eventually)
 */
int main()
{
    Dice dice;
    
    bool playerHasEnteredName = false;
    std::string playerName = "N/A";
    Player player = {Constants::startingBalance, playerName, playerHasEnteredName};
    
    PlayerStats playerStats;
    Game game;
    
    
    std::array<EMinigameType, Constants::minigameAmount> minigameTypes = { EMinigameType::GuessTheDiceSum, EMinigameType::OddOrEven, EMinigameType::YesOrNo, EMinigameType::HigherOrLower, EMinigameType::Roulette };

    constexpr int LOW_STAKES_MIN_BET = 1;
    constexpr int LOW_STAKES_MAX_BET = 30;
    constexpr int HIGH_STAKES_MIN_BET = 50;
    
    int highestBet = player.globalMoney;
    
    std::array<int, 4> standardBets = { LOW_STAKES_MIN_BET, highestBet };
    std::array<int, 4> stakeBets = { LOW_STAKES_MIN_BET, LOW_STAKES_MAX_BET, HIGH_STAKES_MIN_BET, highestBet };
    
    std::array<Minigame, minigameTypes.size()> minigames = { Minigame{stakeBets, true}, Minigame{standardBets, false}, Minigame{stakeBets, true}, Minigame{standardBets, false}, Minigame{standardBets, false} };
    for (int i = 0; i < minigames.size(); i++)
    {
        minigames[i].Initialize(minigameTypes[i]);
    }

    Casino aCasino = {dice, player, playerStats, game, minigames, minigameTypes};

    ResetStats(aCasino);
    ResetBalance(aCasino);
    if (!aCasino.globalGame.globalIsQuitting)
    {
        ChangeState(EStates::MainMenu, aCasino);
    }

    return 0;
}

void RefuseGame(bool& aCantPlay, Casino& casino)
{
    aCantPlay = true;
    WriteLine("As you walk over to the table, the guards immediately escort you away.");
    WriteLine("You start to wonder why. You eventually land on the question 'Maybe I've won too much?'.");
    Pause();
    ClearConsole();
    ChangeState(EStates::Game, casino);
}

void ResetBalance(Casino& aCasino)
{
    aCasino.globalPlayer.globalMoney = Constants::startingBalance;
}

void BroadcastPlayerBalance(bool aStylize, Casino& aCasino)
{
    if (!aCasino.globalGame.globalIsGameOver)
    {
        if (aStylize)
        {
            DrawBreakerLine();
        }

        std::cout << aCasino.globalPlayer.globalPlayerName << " : $" << aCasino.globalPlayer.globalMoney << '\n';
    }
}

bool IsValidName(const std::string& aS)
{
    return aS.size() >= Constants::PLAYER_NAME_MIN_SIZE && aS.size() <= Constants::PLAYER_NAME_MAX_SIZE && strspn(aS.c_str(), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\x8F\x86\x84\x8E\x94\x99") == aS.length();
}

// Resets all stats after the player has lost the entire game.
void ResetGame(Casino& aCasino)
{
    for (Minigame& minigame : aCasino.globalMinigames)
    {
        minigame.Reset();
    }
    
    aCasino.globalPlayerStats.globalWins = 0;
    aCasino.globalPlayerStats.globalLosses = 0;
    aCasino.globalPlayerStats.globalGamesPlayed = 0;
    
    ResetBalance(aCasino);
    ResetStats(aCasino);
}

void AddBalance(int aAmount, int& aMinigame, Casino& aCasino)
{
    aAmount = Min(aAmount, 0);
    aCasino.globalPlayer.globalMoney += aAmount;
    aMinigame += aAmount;
}

void RemoveBalance(int aAmount, int& aMinigame, Casino& aCasino)
{
    aAmount = Min(aAmount, 1);
    aCasino.globalPlayer.globalMoney -= aAmount;
    aCasino.globalPlayer.globalMoney = Min(aCasino.globalPlayer.globalMoney, 0);
    aMinigame += aAmount;
}

void GameOver(Casino& aCasino)
{
    ClearConsole();
    aCasino.globalGame.globalIsGameOver = true;
    Exit(aCasino);

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

void Exit(Casino& aCasino)
{
    aCasino.globalGame.globalIsQuitting = true;
}

void TauntOrImpress(Casino& aCasino, int aWinAmount, int aLossAmount, int aImpressWinAmt, int aTauntLossAmt)
{
    int winLossDifference = aWinAmount - aLossAmount;

    bool shouldBeImpressed = HasExceeded(winLossDifference, aImpressWinAmt);
    bool shouldTaunt = HasSubceded(winLossDifference, -aTauntLossAmt);

    if (shouldBeImpressed)
    {
        std::cout << "Well, " << aCasino.globalPlayer.globalPlayerName << ". Seems like you're on a winning streak! Keep it up!\n";
    }
    else if (shouldTaunt)
    {
        std::cout << "Sad to see you struggling, " << aCasino.globalPlayer.globalPlayerName << ". Can't tell if you're naive or just plain unlucky!\n";
    }
    else if (!shouldBeImpressed && !shouldTaunt)
    {
        std::cout << "Welcome " << aCasino.globalPlayer.globalPlayerName << '\n';
    }
}

// TODO: REPURPOSE
bool AskPlayerAgain(bool aIsInGame, Casino& aCasino)
{
    ClearConsole();

    if (aCasino.globalGame.globalIsGameOver)
    {
        aCasino.globalGame.globalIsGameOver = false;
        ResetGame(aCasino);
        ChangeState(EStates::MainMenu, aCasino);
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
                aCasino.globalGame.globalIsQuitting = true;
            }
            else
            {
                ClearInput();
                ClearConsole();
                aCasino.globalMinigames[aCasino.globalGame.globalCurrentMinigame].EnterGameMenu(aCasino);
            }
            break;
        }

        std::cin >> input;
        isYes = IsCharacter(input, 'Y');
    }

    if (isYes)
    {
        ClearConsole();
        Pick(aCasino.globalGame.globalCurrentMinigame, true, aCasino);
    }
    
    return true;
}

void About(Casino& aCasino)
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
    ChangeState(EStates::MainMenu, aCasino);
}

void CashOut(Casino& aCasino)
{
    int cantPlayAmount = 0;
    for (Minigame minigame : aCasino.globalMinigames)
    {
        if (minigame.myCantPlay) cantPlayAmount++;
    }
    
    // Um... alright.
    if (cantPlayAmount != aCasino.globalMinigames.size())
    {
        WriteLine("'It's too early to cash out' is what your inner voice is telling you.");
        WriteLine("Maybe your inside voice is right...");
        WriteLine("(Get kicked out from every game to cash out!)");
        Pause();
        ClearConsole();
        ChangeState(EStates::Game, aCasino);
        return;
    }

    std::cout << "You decided to cash out $" << aCasino.globalPlayer.globalMoney << '\n';
    Pause();
    WriteLine("Before you walk out the door, you turn around and the game master dead in the eye.");
    Pause();
    WriteLine("Your grin widens, as if you were to mock the casino for everything they put you through.");
    Pause();
    WriteLine("You still didn't change your ways after coming home. You decided to go to a different casino.");
    Pause();
    ClearConsole();
    ResetGame(aCasino);
    ChangeState(EStates::MainMenu, aCasino);
}

void EnterNamePicker(Casino& aCasino)
{
    if (aCasino.globalPlayer.globalHasEnteredName)
    {
        EnterGamePicker(aCasino);
        return;
    }
    
    char confirmName = 'Y';
    char denyName = 'N';
    
    WriteLine("What's your name? (Must be between 2-16 characters)");

    std::string myPlayerNameInput;
    std::cin >> myPlayerNameInput;
    
    while (std::cin.fail() || !IsValidName(myPlayerNameInput))
    {
        WriteLine("Please enter letters only! / Ensure the name is between 2-16 characters.");
        ClearInput();
        std::cin >> myPlayerNameInput;
    }
    
    ClearInput();
    const char* playerName = myPlayerNameInput.c_str();
    
    std::cout << "Your name is: " << playerName  << ". Continue? (y to confirm, n to deny)" << '\n';
    
    char confirmInput;
    std::cin >> confirmInput;
    while (std::cin.fail())
    {
        ClearInput();
        std::cin >> confirmInput;
        WriteLine("Please confirm or deny!");
    }
    
    bool hasConfirmedName = IsCharacter(confirmInput, confirmName);
    bool hasDeniedName = IsCharacter(confirmInput, denyName);
    
    if (!hasConfirmedName && !hasDeniedName)
    {
        std::cin >> confirmInput;
        
        hasConfirmedName = IsCharacter(confirmInput, confirmName);
        hasDeniedName = IsCharacter(confirmInput, denyName);
    }
    
    if (hasConfirmedName)
    {
        aCasino.globalPlayer.globalHasEnteredName = true;
        aCasino.globalPlayer.globalPlayerName = myPlayerNameInput;
        
        ClearConsole();
        EnterGamePicker(aCasino);
    }
    else if (hasDeniedName)
    {
        ClearConsole();
        EnterNamePicker(aCasino);
    }
}

void EnterGamePicker(Casino& aCasino)
{
    const int OPTIONS = int(aCasino.globalMinigameTypes.size()) + 2;
    
    int input;
    
    // Sooner or later I'll have to change this based on 
    DrawTitle("GAME PICKER");
    
    int minigameCount = int(aCasino.globalMinigameTypes.size());
    
    for (int i = 0; i < minigameCount; i++)
    {
        Minigame minigame = aCasino.globalMinigames[i];
        EMinigameType minigameType = aCasino.globalMinigameTypes[i];
        
        std::cout << "[" << (i + 1) << "] " << minigame.FromMinigameToChar(minigameType) << '\n';
    }
    
    DrawBreakerLine();
    
    std::cout << "[" << (minigameCount + 1) << "] " << "Cash Out" << '\n';
    std::cout << "[" << (minigameCount + 2) << "] " << "Back To Menu" << '\n';
    
    DrawMenuLine();
    
    BroadcastPlayerBalance(false, aCasino);
    
    ForceInput(input);
    
    input = Clamp(input, 1, OPTIONS);
    
    Pick(input, true, aCasino);
}

void EnterMainMenu(Casino& aCasino)
{
    int input;
    DrawMenu(aCasino, input, "THE ULTIMATE CASINO", "[1] Play Game\n[2] About\n[3] Stats\n[4] Quit", 4);
    Pick(input, false, aCasino);
}

void SwitchTo(EOptions aOption, Casino& aCasino)
{
    switch (aOption)
    {
    case EOptions::CashOut:
        CashOut(aCasino);
        break;
    case EOptions::Stats:
        ShowStats(aCasino);
        break;
    case EOptions::About:
        About(aCasino);
        break;
    case EOptions::Quit:
        Exit(aCasino);
        break;
    }
}

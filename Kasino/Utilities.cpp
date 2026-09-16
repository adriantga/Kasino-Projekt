#include <iostream>
#include "Helpers.h"
#include <random>
#include "Utilities.h"

#include "Kasino.h"
#include "Stats.h"

constexpr int globalLowerCaseOffset = 32;

// Initialize random number generator
std::random_device rnd;
std::mt19937 distributor(rnd());

int GetRandomNumber(int aMin, int aMax)
{
    std::uniform_int_distribution<int> generator(aMin, aMax);
    return generator(distributor);
}

void DrawTitle(const char aTitleText[])
{
    DrawMenuLine();
    WriteLine(aTitleText);
    DrawMenuLine();
}

int GetBetAmount(Player& aPlayer)
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
    result = Clamp(result, 1, aPlayer.myMoney);
    return result;
}

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

int RollDie()
{
    return GetRandomNumber(1, 6);
}

void DrawBreakerLine(bool aNewLine)
{
    WriteLine("--------------------------------------", aNewLine);
}

void DrawMenuLine()
{
    WriteLine("===========================================================");
}

void RollDice(Dice& aDice)
{
    aDice.die1 = RollDie();
    aDice.die2 = RollDie();
    aDice.diceSum = aDice.die1 + aDice.die2;
}

bool IsCharacter(char aInput, char aValue)
{
    return aInput == aValue || ToLower(aValue);
}

int GetRoll()
{
    return GetRandomNumber(1, 100);
}

void ClearInput()
{
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}

int Max(int aValue, int aMax)
{
    if (aValue > aMax)
    {
        return aMax;
    }

    return aValue;
}

int Min(int aValue, int aMin)
{
    if (aValue < aMin)
    {
        return aMin;
    }

    return aValue;
}

int Clamp(int aValue, int aMin, int aMax)
{
    return Max(Min(aValue, aMin), aMax);
}

bool IsInRange(int aValue, int aMin, int aMax)
{
    return aValue >= aMin && aValue <= aMax;
}

bool HasSubceded(int aCurrent, int aTarget)
{
    return aCurrent < aTarget;
}

bool HasExceeded(int aCurrent, int aTarget)
{
    return aCurrent > aTarget;
}

bool IsEven(int aValue)
{
    return aValue % 2 == 0;
}

void ClearConsole()
{
    system("cls");
}

void Pause()
{
    system("pause");
}

void WriteLine(const char aTextToType[], bool aNewLine)
{
    std::cout << aTextToType;
    if (aNewLine) std::cout << '\n';
}

void BroadcastDiceResult(Dice& aDice, bool aShowSum)
{
    WriteLine("--------------- RESULT ---------------");

    std::cout << "DIE 1 - " << aDice.die1 << std::endl;
    std::cout << "DIE 2 - " << aDice.die2 << std::endl;

    if (aShowSum)
    {
        std::cout << "SUM - " << aDice.diceSum << std::endl;
    }

    DrawBreakerLine();
}

bool ShouldShowInstructions(const Game aGame, const bool hasPlayed)
{
    return aGame.isShowingInstructions && !hasPlayed;
}

std::array<int, 12> BuildRow(int aRowStart)
{
    std::array<int, 12> result; 
    
    int num = aRowStart;
    
    for (int i = 0; i < result.size(); i++)
    {
        result[i] = num;
        num += 3;
    }
    
    return result;
}

void BroadcastWinOrLoss(Player& aPlayer, PlayerStats& aStats, bool aIsWinner, int aWinAmount, int aLoseAmount,
                        int& aMinigame)
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

        if (aPlayer.myMoney == 0)
        {
            Pause();
            GameOver();
        }
    }

    AddPlayedGame(aStats, aIsWinner);
    BroadcastPlayerBalance(true);

    Pause();
    AskPlayerAgain(true);
}

bool IsMatching(int current[], int target[])
{
    return current == target;
}

char ToLower(char aCharacter)
{
    return aCharacter + globalLowerCaseOffset;
}

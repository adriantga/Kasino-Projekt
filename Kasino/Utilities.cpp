#include <iostream>
#include "Helpers.h"
#include <random>
#include "Utilities.h"

const int globalLowerCaseOffset = 32;

// Initialize random number generator
std::random_device rnd;
std::mt19937 distributor(rnd());

int GetRandomNumber(int aMin, int aMax)
{
    std::uniform_int_distribution<int> generator(aMin, aMax);
    return generator(distributor);
}

int RollDie()
{
    return GetRandomNumber(1, 6);
}

void RollDice(Dice& aDice)
{
    aDice.die1 = RollDie();
    aDice.die2 = RollDie();
    aDice.diceSum = aDice.die1 + aDice.die2;
}

bool IsCharacter(char aInput, char aValue)
{
    return aInput == aValue || aInput == aValue + globalLowerCaseOffset;
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

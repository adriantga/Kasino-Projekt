#pragma once
#include <array>

#include "Minigame.h"

enum class EMinigame;
class Minigame;
struct Constants;

struct Constants
{
    static const int minigameAmount = 5;
};

struct Player
{
    int myMoney = 0;
    int pickedMinigame = -1;
};

struct PlayerStats
{
    const int matchesDisplayed = 6;
    const int winIndex = 0;
    const int lossIndex = 1;

    int wins = 0;
    int losses = 0;
    int matchesPlayed = 0;

    char matches[5] = {'-', '-', '-', '-', '-'};
    const char matchResults[2] = {'W', 'L'};
};

struct Dice
{
    const int diceSumMin = 2;
    const int diceSumMax = 12;

    int die1 = 0;
    int die2 = 0;
    int diceSum = 0;
};

struct Game
{
    const int ROULETTE_BETTING_TYPE_MIN = 1;
    const int ROULETTE_BETTING_TYPE_MAX = 4;
    
    const int ROULETTE_STRAIGHT_MIN = 0;
    const int ROULETTE_STRAIGHT_MAX = 36;
    
    const int ROULETTE_STRAIGHT = 1;
    const int ROULETTE_RED_BLACK = 2;
    const int ROULETTE_ODD_EVEN = 3;
    const int ROULETTE_COLUMN_ROW = 4;
    
    const int ROULETTE_ROW_MIN = 1;
    const int ROULETTE_ROW_MAX = 3;
    
    const int ROULETTE_COLUMN_MIN = 0;
    const int ROULETTE_COLUMN_MAX = 12;
    
    const int startingBalance = 250;
    
    const char validRoulettePicks[3] = { 'r', 'g', 'b' };

    bool isGameOver = false;
    bool isQuitting = false;

    bool isShowingInstructions = true;
};

struct Casino
{
    Dice& dice;
    Player& player;
    PlayerStats& playerStats;
    Game& game;
    
    std::array<bool, Constants::minigameAmount>& shouldShowInstructions;
    std::array<EMinigame, Constants::minigameAmount>& minigameTypes;
    std::array<Minigame, Constants::minigameAmount>& minigames;
};

enum class EOptions
{
    CashOut,
    Stats,
    About,
    Quit
};

enum class EMinigame
{
    GuessTheDiceSum = 1,
    OddOrEven = 2,
    YesOrNo = 3,
    HigherOrLower = 4,
    Roulette = 5
};

enum class EStates
{
    MainMenu,
    Game
};

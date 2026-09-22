#pragma once
#include <array>
#include <string>

enum class EMinigameType;
class Minigame;
struct Constants;

// Unsure of what's considered a global variable in this case, so I'll assume
// the source is this specific header.

struct Constants
{
    static const int minigameAmount = 5;
    static const int startingBalance = 250;
    static const int PLAYER_NAME_MIN_SIZE = 2;
    static const int PLAYER_NAME_MAX_SIZE = 16;
};

struct Player
{
    int globalMoney = 0;
    std::string& globalPlayerName;
    bool& globalHasEnteredName;
};

struct PlayerStats
{
    const int winIndex = 0;
    const int lossIndex = 1;

    int globalWins = 0;
    int globalLosses = 0;
    int globalGamesPlayed = 0;

    char globalGames[5] = {'-', '-', '-', '-', '-'};
    const char gameOutcomes[2] = {'W', 'L'};
};

struct Dice
{
    const int diceSumMin = 2;
    const int diceSumMax = 12;

    int globalDie1 = 0;
    int globalDie2 = 0;
    int globalDiceSum = 0;
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
    
    const char validRoulettePicks[3] = { 'r', 'g' };
    int globalCurrentMinigame;
    
    bool globalIsGameOver = false;
    bool globalIsQuitting = false;
};

struct Casino
{
    Dice& globalDice;
    Player& globalPlayer;
    PlayerStats& globalPlayerStats;
    Game& globalGame;
    std::array<Minigame, Constants::minigameAmount>& globalMinigames;
    std::array<EMinigameType, Constants::minigameAmount>& globalMinigameTypes;
};

enum class EOptions
{
    CashOut,
    Stats,
    About,
    Quit
};

enum class EMinigameType
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

enum class EStakeType
{
    High,
    Low
};
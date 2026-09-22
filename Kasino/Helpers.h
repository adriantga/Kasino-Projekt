#pragma once
#include <array>
#include <string>

enum class EMinigameType;
class Minigame;
struct Constants;

struct Constants
{
    static const int MINIGAME_AMOUNT = 5;
    static const int STARTING_BALANCE = 250;
    static const int PLAYER_NAME_MIN_SIZE = 2;
    static const int PLAYER_NAME_MAX_SIZE = 16;
};

struct Player
{
    int money = 0;
    std::string& name;
    bool& hasEnteredName;
    
    std::array<int, 3> standardBets;
    std::array<int, 3> stakeBets;
};

struct PlayerStats
{
    const int WIN_INDEX = 0;
    const int LOSS_INDEX = 1;

    int wins = 0;
    int losses = 0;
    int gamesPlayed = 0;

    char games[5] = {'-', '-', '-', '-', '-'};
    const char GAME_OUTCOMES[2] = {'W', 'L'};
};

struct Dice
{
    const int DICE_SUM_MIN = 2;
    const int DICE_SUM_MAX = 12;

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

    const char VALID_ROULETTE_PICKS[3] = {'r', 'g'};
    int currentMinigame;

    bool isGameOver = false;
    bool isQuitting = false;
};

struct Casino
{
    Dice& dice;
    Player& player;
    PlayerStats& playerStats;
    Game& game;
    std::array<Minigame, Constants::MINIGAME_AMOUNT>& minigames;
    std::array<EMinigameType, Constants::MINIGAME_AMOUNT>& minigameTypes;
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

#pragma once

struct Player
{
    int myMoney = 0;

    bool hasPlayedYesOrNo = false;
    bool hasPlayedDiceSum = false;
    bool hasPlayedOddOrEven = false;

    bool cantPlayDiceSum = false;
    bool cantPlayOddEven = false;
    bool cantPlayYesNo = false;

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

    int diceSumWinAmount = 0;
    int oddEvenWinAmount = 0;
    int yesNoWinAmount = 0;

    int diceSumLossAmount = 0;
    int oddEvenLossAmount = 0;
    int yesNoLossAmount = 0;
};

struct Dice
{
    const int diceSumMin = 2;
    const int diceSumMax = 12;

    int die1 = 0;
    int die2 = 0;
    int diceSum = 0;
};

struct Rewards
{
    const int yesNoBaseReward = 25;

    const int guessTheSumRewardMultiplier = 2;
    const int oddOrEvenRewardMultiplier = 2;
    int yesNoRewardMultiplier = 1;
};

struct Game
{
    // The amount of money in total you have to win for the casino to kick you out.
    const int diceSumWinLimit = 5000;
    const int oddEvenWinLimit = 5000;
    const int yesNoWinLimit = 5000;

    // The money difference between losses and wins for the casino to taunt you.
    const int diceSumTauntLossAmt = 1000;
    const int oddEvenTauntLossAmt = 1000;
    const int yesNoTauntLossAmt = 1000;

    // The money difference between wins and losses for the casino to impress you.
    const int diceSumImpressWinAmt = 500;
    const int oddEvenImpressWinAmt = 500;
    const int yesNoImpressWinAmt = 500;

    const int startingBalance = 250;

    bool isGameOver = false;
    bool isQuitting = false;

    bool isShowingInstructions = true;
};

enum class EOptions
{
    GuessTheDiceSum,
    OddOrEven,
    YesOrNo,
    CashOut,
    Stats,
    About,
    Quit
};

enum class EStates
{
    MainMenu,
    Game
};

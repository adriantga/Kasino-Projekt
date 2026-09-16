#pragma once

struct Player
{
    int myMoney = 0;

    bool hasPlayedYesOrNo = false;
    bool hasPlayedDiceSum = false;
    bool hasPlayedOddOrEven = false;
    bool hasPlayedHigherOrLower = false;
    bool hasPlayedRoulette = false;

    bool cantPlayDiceSum = false;
    bool cantPlayOddEven = false;
    bool cantPlayYesNo = false;
    bool cantPlayHigherLower = false;
    bool cantPlayRoulette = false;

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
    int higherLowerWinAmount = 0;
    int rouletteWinAmount = 0;

    int diceSumLossAmount = 0;
    int oddEvenLossAmount = 0;
    int yesNoLossAmount = 0;
    int higherLowerLossAmount = 0;
    int rouletteLossAmount = 0;
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
    // The base reward given!
    const int yesNoBaseReward = 25;
    
    // This feels just right
    const int higherLowerBaseReward = 50;
    
    // Felt like 2x was too low, so it's been bumped up to 5x!
    const int guessTheSumRewardMultiplier = 5;
    
    // Make it a bit more generous with the odd or even reward
    const int oddOrEvenRewardMultiplier = 4;
    
    // Unlike the yes or no minigame, this one will be more of a 
    // one-round thing(similar to guess the dice sum and even or odd)
    const int higherLowerRewardMultiplier = 2;
    
    const int rouletteRewardMultiplier = 3;
    
    // This will dynamically change
    int yesNoRewardMultiplier = 1;
};

struct Game
{
    // The amount of money in total you have to win for the casino to kick you out.
    const int diceSumWinLimit = 2500;
    const int oddEvenWinLimit = 1500;
    const int yesNoWinLimit = 4000;
    const int higherLowerWinLimit = 3000;
    const int rouletteWinLimit = 5000;

    // The money difference between losses and wins for the casino to taunt you.
    const int diceSumTauntLossAmt = 500;
    const int oddEvenTauntLossAmt = 500;
    const int yesNoTauntLossAmt = 500;
    const int higherLowerTauntLossAmt = 500;
    const int rouletteTauntLossAmt = 500;

    // The money difference between wins and losses for the casino to impress you.
    const int diceSumImpressWinAmt = 500;
    const int oddEvenImpressWinAmt = 500;
    const int yesNoImpressWinAmt = 500;
    const int higherLowerImpressWinAmt = 500;
    const int rouletteImpressWinAmt = 500;
    
    const int startingBalance = 250;
    
    const char validRoulettePicks[3] = { 'r', 'g', 'b' };

    bool isGameOver = false;
    bool isQuitting = false;

    bool isShowingInstructions = true;
};

enum class EOptions
{
    GuessTheDiceSum,
    OddOrEven,
    YesOrNo,
    HigherOrLower,
    Roulette,
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

// For later use-case
enum class ERouletteBetTypes
{
    Straight,
    RedBlack,
    OddEven,
    ColumnBet
};
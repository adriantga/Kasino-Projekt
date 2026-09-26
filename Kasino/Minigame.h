#pragma once

#include "Helpers.h"

struct Constants;
struct Casino;
enum class EMinigameType;

class Minigame
{
    const int MINIGAME_GUESS_THE_DICE_SUM{0};
    const int MINIGAME_ODD_OR_EVEN = {1};
    const int MINIGAME_YES_OR_NO = {2};
    const int MINIGAME_HIGHER_OR_LOWER = {3};
    const int MINIGAME_ROULETTE = {4};

    const int LOW_NO_STAKES_MIN = {0};
    const int LOW_NO_STAKES_MAX = {1};
    const int HIGH_STAKES_MIN = {2};

    int myRewardMultiplier = 1;
    int myWinImpressAmount = 500;
    int myLossTauntAmount = 500;
    int myRewardIncreaseThreshold = 0;

    int myCachedReward = 0;
    bool myUseCachedReward = false;

    // Used by 'Roulette' and 'Yes or No'
    int myRangeStart = 0;
    int myRangeEnd = 0;

    char myFirstChoice = ' ';
    char mySecondChoice = ' ';
    char myQuitChoice = 'Q';

    // Only used by 'Roulette' at the moment
    int myAlternativeRewardMultiplier = 1;
    int myBaseReward = 0;

    bool myShouldPlayerQuit = false;
    bool myIsPlayerQuitting = false;
    bool myIsForcedVictory = false;
    bool myIsForcedWinAmount = true;

    bool myHasPlayerPickedFirst = false;
    bool myHasPlayerPickedSecond = false;
    
    bool myIsPlayingHighStakes = false;

    EMinigameType myMinigameType;

    void PlayGame(Casino& aCasino);
    void ShowRules(Casino& aCasino);
    void OnPlay(Casino& aCasino);

public:
    int myWinAmount = 0;
    int myLossAmount = 0;
    int myWinLimit = 0;

    int myBet = 0;
    
    // I store 3 values because 2 are for low-stakes/no stakes, and 1 is for high stakes.
    std::array<int, 3> myAllowedBets = {0, 0};
    int myMinAllowedBet = 0;
    int myMaxAllowedBet = 0;

    bool myHasStakes = false;

    int myGameIndex = 0;

    bool myCantPlay = false;

    Minigame(Player& aPlayer, std::array<int, 3>& aAllowedBets, bool aHasStakes);
    
    
    bool IsPlayingHighStakes() const { return myIsPlayingHighStakes; }
    void Initialize(const EMinigameType& aMinigameType);
    void EnterGameMenu(Casino& aCasino);
    void UpdateBets(Player& aPlayer);

    void Reset();
};

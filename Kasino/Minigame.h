#pragma once

#include "Helpers.h"

struct Constants;
struct Casino;
enum class EMinigameType;

class Minigame
{
    const int MINIGAME_GUESS_THE_DICE_SUM { 0 };
    const int MINIGAME_ODD_OR_EVEN = { 1 };
    const int MINIGAME_YES_OR_NO = { 2 };
    const int MINIGAME_HIGHER_OR_LOWER = { 3 };
    const int MINIGAME_ROULETTE = { 4 };
    
    int myRewardMultiplier = 1;
    int myWinImpressAmount = 500;
    int myLossTauntAmount = 500;
    int myRewardIncreaseThreshold = 0;

    int myCachedReward = 0;
    bool myUseCachedReward = false;

    // Used by 'Roulette' and 'Yes or No' but differently!
    int myRangeStart = 0;
    int myRangeEnd = 0;

    // Only used by 'Roulette'
    int myAlternativeRewardMultiplier = 1;
    int myBaseReward = 0;
    
    bool myPlayerCanQuit = false;
    bool myIsPlayerQuitting = false;
    bool myIsForcedVictory = false;
    bool myIsForcedWinAmount = true;
    
    char myFirstChoice = ' ';
    char mySecondChoice = ' ';
    char myQuitChoice = 'Q';
    
    bool myHasPlayerPickedFirst = false;
    bool myHasPlayerPickedSecond = false;
    
    bool myHasStakes = false;
    
    EMinigameType myMinigameType;
    
    // METHODS
    void ShowInstructions();
    void OnPlay(Casino& aCasino);

public:
    int myWinAmount = 0;
    int myLossAmount = 0;
    int myWinLimit = 0;
    
    int myBet = 0;
    int myMinAllowedBet = 0;
    int myMaxAllowedBet = 0;
    
    const char *FromMinigameToChar(EMinigameType& aMinigame);

    int myGameIndex = 0;
    
    bool myCantPlay = false;
    
    void Initialize(EMinigameType& aMinigame);
    void PlayGame(Casino &aCasino);
    
    void Reset();
};
#include <iostream>
#include "Minigames.h"
#include "Kasino.h"
#include "Utilities.h"

namespace GuessTheDiceSum
{
    void PlayGame(Dice& aDice, Game& aGame, Player& aPlayer, PlayerStats& aPlayerStats, Rewards& aRewards)
    {
        if (HasExceeded(aPlayerStats.diceSumWinAmount, aGame.diceSumWinLimit))
        {
            RefuseGame(aPlayer.cantPlayDiceSum);
            return;
        }

        int bet = GetBetAmount(aPlayer);

        RollDice(aDice);

        TauntOrImpress(aPlayerStats.diceSumWinAmount, aPlayerStats.diceSumLossAmount, aGame.diceSumImpressWinAmt,
                       aGame.diceSumTauntLossAmt);
        if (aGame.isShowingInstructions && !aPlayer.hasPlayedDiceSum)
        {
            std::cout <<
                "Your only goal is to guess the sum of the dice. Your guess should not exceed 12 or fall behind 2!\nIf you do end exceeding or falling behind the boundary, we'll assume you mean 2 or 12."
                << std::endl;
        }

        aPlayer.hasPlayedDiceSum = true;
        std::cout << "Guess a number between 2-12:" << std::endl;
        int playerGuess;
        std::cin >> playerGuess;

        while (std::cin.fail())
        {
            ClearInput();
            std::cin >> playerGuess;
        }

        playerGuess = Clamp(playerGuess, aDice.diceSumMin, aDice.diceSumMax);
        std::cout << playerGuess << std::endl;

        BroadcastDiceResult(aDice, true);

        bool isWinner = playerGuess == aDice.diceSum;

        int winAmount = bet * aRewards.guessTheSumRewardMultiplier;

        BroadcastWinOrLoss(aPlayer, aPlayerStats, isWinner, winAmount, bet,
                           isWinner ? aPlayerStats.diceSumWinAmount : aPlayerStats.diceSumLossAmount);
    }
}

namespace OddOrEven
{
    void PlayGame(Dice& aDice, Game& aGame, Player& aPlayer, PlayerStats& aPlayerStats, Rewards& aRewards)
    {
        if (HasExceeded(aPlayerStats.oddEvenWinAmount, aGame.oddEvenWinLimit))
        {
            RefuseGame(aPlayer.cantPlayOddEven);
            return;
        }


        int bet = GetBetAmount(aPlayer);

        RollDice(aDice);

        TauntOrImpress(aPlayerStats.oddEvenWinAmount, aPlayerStats.oddEvenLossAmount, aGame.oddEvenImpressWinAmt,
                       aGame.oddEvenTauntLossAmt);

        if (aGame.isShowingInstructions && !aPlayer.hasPlayedOddOrEven)
        {
            std::cout <<
                "Here you must guess if the dice are even or odd. If both dice aren't even or odd, the house wins.";
        }

        aPlayer.hasPlayedOddOrEven = true;

        std::cout << "What are you guessing? Even or odd?\n1. Even\n2. Odd" << std::endl;

        int picked; // This will be replaced by 'char' in "Uppgift 3"
        std::cin >> picked;

        while (std::cin.fail())
        {
            ClearInput();
            std::cin >> picked;
        }

        picked = Clamp(picked, 1, 2);

        bool isEvenPicked = (picked == 1);

        bool isEven = IsEven(aDice.die1) && IsEven(aDice.die2);
        bool isOdd = !IsEven(aDice.die1) && !IsEven(aDice.die2);
        bool isWinner = (isEvenPicked && isEven) || (!isEvenPicked && isOdd);

        BroadcastDiceResult(aDice, false);

        int winAmount = bet * aRewards.oddOrEvenRewardMultiplier;

        BroadcastWinOrLoss(aPlayer, aPlayerStats, isWinner, winAmount, bet,
                           isWinner ? aPlayerStats.oddEvenWinAmount : aPlayerStats.oddEvenLossAmount);
    }
}

namespace YesOrNo
{
    void PlayGame(Game& aGame, Player& aPlayer, PlayerStats& aPlayerStats, Rewards& aRewards)
    {
        if (HasExceeded(aPlayerStats.yesNoWinAmount, aGame.yesNoWinLimit))
        {
            RefuseGame(aPlayer.cantPlayYesNo);
            return;
        }

        aRewards.yesNoRewardMultiplier = 1;

        // Store 'Y' and 'N' to prevent the code from being too repetitive
        char yes = 'Y';
        char no = 'N';

        int bet = GetBetAmount(aPlayer);

        int rewardMultiplierIncreaseThreshold = 3;
        int numConsecutiveCorrectGuesses = 0;

        int rangeStart = GetRoll();
        int rangeEnd = GetRoll();
        int actualRoll = GetRoll();

        if (rangeStart > rangeEnd)
        {
            int temp = rangeEnd;
            rangeStart = rangeEnd;
            rangeEnd = temp;
        }

        if (rangeStart == rangeEnd)
        {
            rangeEnd++;
        }

        TauntOrImpress(aPlayerStats.yesNoWinAmount, aPlayerStats.yesNoLossAmount, aGame.yesNoImpressWinAmt,
                       aGame.yesNoTauntLossAmt);
        if (aGame.isShowingInstructions && !aPlayer.hasPlayedYesOrNo)
        {
            std::cout << "Your sole objective is to guess whether the number picked is within the given range" <<
                std::endl;
            std::cout << "Use 'y' if you think the number is in range and 'n' to guess if it is not." << std::endl;
            std::cout << "Every time you have " << rewardMultiplierIncreaseThreshold <<
                " correct guesses in a row, your reward might increase!" << std::endl;
            std::cout << "** NOTE ** If you quit before guessing, you'll automatically lose." << std::endl;
        }

        aPlayer.hasPlayedYesOrNo = true;


        std::cout << "Is the number between " << rangeStart << " and " << rangeEnd << "?" << std::endl;

        int cachedReward = 0;
        int winAmount = 0;

        char playerInput;
        std::cin >> playerInput;
        bool playerPickedYes = IsCharacter(playerInput, yes);
        bool playerPickedNo = IsCharacter(playerInput, no);

        bool isPlayerQuitting = IsCharacter(playerInput, 'q');
        while (!isPlayerQuitting)
        {
            while (std::cin.fail())
            {
                ClearInput();
                std::cin >> playerInput;
            }

            if (playerPickedYes || playerPickedNo)
            {
                if (playerPickedNo && !IsInRange(actualRoll, rangeStart, rangeEnd) || playerPickedYes && IsInRange(
                    actualRoll, rangeStart, rangeEnd))
                {
                    numConsecutiveCorrectGuesses++;
                    if (numConsecutiveCorrectGuesses >= rewardMultiplierIncreaseThreshold)
                    {
                        numConsecutiveCorrectGuesses = 0;
                        aRewards.yesNoRewardMultiplier++;
                        std::cout << "Your reward multiplier has increased! It is now " << aRewards.
                            yesNoRewardMultiplier << "!" << std::endl;
                    }

                    actualRoll = GetRoll();
                    rangeStart = GetRoll();
                    rangeEnd = GetRoll();

                    if (rangeStart > rangeEnd)
                    {
                        int temp = rangeEnd;
                        rangeStart = rangeEnd;
                        rangeEnd = temp;
                    }

                    if (rangeStart == rangeEnd)
                    {
                        rangeEnd++;
                    }

                    std::cout << "Is the number between " << rangeStart << " and " << rangeEnd << "?" << std::endl;

                    cachedReward += aRewards.yesNoBaseReward;
                }
                else
                {
                    cachedReward = 0; // Reset the cached reward if the player guesses wrong
                    std::cout << "You guessed wrong! The number was " << actualRoll << " which means it was " << (
                        IsInRange(actualRoll, rangeStart, rangeEnd) ? "in" : "not in") << " the range." << std::endl;
                    break;
                }
            }

            std::cin >> playerInput;

            playerPickedYes = IsCharacter(playerInput, yes);
            playerPickedNo = IsCharacter(playerInput, no);
            isPlayerQuitting = IsCharacter(playerInput, 'q');
        }

        if (cachedReward > 0)
        {
            winAmount = bet + (cachedReward * aRewards.yesNoRewardMultiplier);
        }

        bool isWinner = winAmount > 0 && isPlayerQuitting;

        BroadcastWinOrLoss(aPlayer, aPlayerStats, isWinner, winAmount, bet,
                           isWinner ? aPlayerStats.yesNoWinAmount : aPlayerStats.yesNoLossAmount);
    }
}

namespace HigherOrLower
{
    void PlayGame(Game& aGame, Player& aPlayer, PlayerStats& aPlayerStats, Rewards& aRewards)
    {
        if (HasExceeded(aPlayerStats.higherLowerWinAmount, aGame.higherLowerWinLimit))
        {
            RefuseGame(aPlayer.cantPlayHigherLower);
            return;
        }
        
        int bet = GetBetAmount(aPlayer);
        
        int pickedNumber = GetRoll();
        int nextPickedNumber = GetRoll();
        
        while (pickedNumber == nextPickedNumber)
        {
            pickedNumber = GetRoll();
        }
        
        TauntOrImpress(aPlayerStats.yesNoWinAmount, aPlayerStats.yesNoLossAmount, aGame.yesNoImpressWinAmt,
                       aGame.yesNoTauntLossAmt);
        if (aGame.isShowingInstructions && !aPlayer.hasPlayedYesOrNo)
        {
            WriteLine("The goal is simple: Guess whether or not the number visible on the screen is lower than the next one");
            WriteLine("Guess correctly and you'll get rewarded. However, a wrong guess will cost you everything!");
            WriteLine("(By quitting, you indirectly forfeit any possible winnings, thus the round becoming a loss)");
        }
        
        std::cout << "Do you think " << pickedNumber << " is higher or lower than the next number?" << '\n';
        
        bool isHigher = pickedNumber > nextPickedNumber;
        
        char higher = 'H';
        char lower = 'L';
        
        char playerGuess;
        std::cin >> playerGuess;
        
        bool playerPickedHigher = IsCharacter(playerGuess, higher);
        bool playerPickedLower = IsCharacter(playerGuess, lower);
        
        // Reward caches if the game master isn't scared.
        int cachedReward = 0;
        int winAmount = 0;
        
        bool isPlayerQuitting = IsCharacter(playerGuess, 'Q');
        
        while (!isPlayerQuitting)
        {
            while (std::cin.fail())
            {
                ClearInput();
                std::cin >> playerGuess;
            }
            
            if (playerPickedHigher || playerPickedLower)
            {
                if (playerPickedHigher && isHigher || playerPickedLower && !isHigher)
                {
                    // It's flipped hehe
                    std::cout << "That's correct! " << pickedNumber << " is " << (isHigher ? "higher" : "lower") << " than " << nextPickedNumber << "!" << "\n";
                    cachedReward += aRewards.higherLowerBaseReward;
                    
                    int temp = nextPickedNumber;
                    pickedNumber = temp;
                    nextPickedNumber = GetRoll();
        
                    while (pickedNumber == nextPickedNumber)
                    {
                        pickedNumber = GetRoll();
                    }
            
                    isHigher = pickedNumber > nextPickedNumber;
                    
                    std::cout << "Do you think " << pickedNumber << " is higher or lower than the next number?" << '\n';
                }
                else
                {
                    std::cout << "The number after " << pickedNumber << " was " << nextPickedNumber << "!\n";
                    WriteLine("Better luck next time!");
                    cachedReward = 0;
                    break;
                }
            }
            
            std::cin >> playerGuess;
            playerPickedHigher = IsCharacter(playerGuess, higher);
            playerPickedLower = IsCharacter(playerGuess, lower);
            isPlayerQuitting = IsCharacter(playerGuess, 'Q');
            
        }
        
        
        if (cachedReward > 0)
        {
            winAmount = bet + (cachedReward * aRewards.higherLowerRewardMultiplier);
        }

        bool isWinner = winAmount > 0 && isPlayerQuitting;

        BroadcastWinOrLoss(aPlayer, aPlayerStats, isWinner, winAmount, bet,
                           isWinner ? aPlayerStats.higherLowerWinAmount : aPlayerStats.higherLowerLossAmount);
    }
}
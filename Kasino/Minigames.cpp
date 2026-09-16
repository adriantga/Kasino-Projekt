#include <iostream>
#include "Minigames.h"
#include "Kasino.h"
#include "Utilities.h"
#include <array>

// My code is highkey ass
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
        if (ShouldShowInstructions(aGame, aPlayer.hasPlayedDiceSum))
        {
            WriteLine("Your only goal is to guess the sum of the dice. Your guess should not exceed 12 or fall behind 2!\nIf you do end exceeding or falling behind the boundary, we'll assume you mean 2 or 12.");
        }

        aPlayer.hasPlayedDiceSum = true;
        WriteLine("Guess a number between 2-12:");
        int playerGuess;
        std::cin >> playerGuess;

        while (std::cin.fail())
        {
            ClearInput();
            std::cin >> playerGuess;
        }

        playerGuess = Clamp(playerGuess, aDice.diceSumMin, aDice.diceSumMax);

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
        
        char even = 'E';
        char odd = 'O';

        RollDice(aDice);

        TauntOrImpress(aPlayerStats.oddEvenWinAmount, aPlayerStats.oddEvenLossAmount, aGame.oddEvenImpressWinAmt,
                       aGame.oddEvenTauntLossAmt);

        if (ShouldShowInstructions(aGame, aPlayer.hasPlayedOddOrEven))
        {
            WriteLine("Here you must guess if the dice are even or odd. If both dice aren't even or odd, the house wins.");
        }

        aPlayer.hasPlayedOddOrEven = true;

        WriteLine("What are you guessing? Even or odd? Type 'e' for even or 'o' for odd!");

        char picked;
        std::cin >> picked;

        bool playerPickedEven = IsCharacter(picked, even);
        bool playerPickedOdd = IsCharacter(picked, odd);
        
        bool isEven = IsEven(aDice.die1) && IsEven(aDice.die2);
        bool isOdd = !IsEven(aDice.die1) && !IsEven(aDice.die2);
        
        while (!playerPickedEven && !playerPickedOdd)
        {
            while (std::cin.fail())
            {
                ClearInput();
                std::cin >> picked;
            }
            
            std::cin >> picked;
            playerPickedEven = IsCharacter(picked, even);
            playerPickedOdd = IsCharacter(picked, odd);
        }
        
        bool isWinner = (playerPickedEven && isEven) || (playerPickedOdd && isOdd);
        
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
        if (ShouldShowInstructions(aGame, aPlayer.hasPlayedYesOrNo))
        {
            WriteLine("Your sole objective is to guess whether the number picked is within the given range");
            WriteLine("Use 'y' if you think the number is in range and 'n' to guess if it is not.");
            std::cout << "Every time you have " << rewardMultiplierIncreaseThreshold <<
                " correct guesses in a row, your reward might increase!" << std::endl;
            WriteLine("** NOTE ** If you quit before guessing, you'll automatically lose.");
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

        TauntOrImpress(aPlayerStats.higherLowerWinAmount, aPlayerStats.higherLowerLossAmount, aGame.higherLowerImpressWinAmt,
                       aGame.higherLowerTauntLossAmt);
        if (ShouldShowInstructions(aGame, aPlayer.hasPlayedHigherOrLower))
        {
            WriteLine(
                "The goal is simple: Guess whether or not the number visible on the screen is lower than the next one");
            WriteLine("Guess correctly and you'll get rewarded. However, a wrong guess will cost you everything!");
            WriteLine("(By quitting, you indirectly forfeit any possible winnings, thus the round becoming a loss)");
        }
        
        aPlayer.hasPlayedHigherOrLower = true;

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
                    std::cout << "That's correct! " << pickedNumber << " is " << (isHigher ? "higher" : "lower") <<
                        " than " << nextPickedNumber << "!" << "\n";
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

/*
* Bets:
* 0-36
* 
* Red/Black:
* Red - Bets on red
* Black - Bets on black
* Odd/Even - Bets on even/odd (how do I even begin to implement this?)
* 
* Column/Bet:
* 
* Row 1[1, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31, 34]
* Row 2[2, 5, 8, 11, 14, 17, 20, 23, 26, 29, 32, 35]
* Row 3[3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36]
 */

// Betting Types:
// Straight
// Red/Black
// Odd/Even
// Column Bet
namespace Roulette
{
    void PlayGame(Game& aGame, Player& aPlayer, PlayerStats& aPlayerStats, Rewards& aRewards)
    {
        if (HasExceeded(aPlayerStats.rouletteWinAmount, aGame.rouletteWinLimit))
        {
            RefuseGame(aPlayer.cantPlayRoulette);
            return;
        }
        
        int bet = GetBetAmount(aPlayer);
        
        TauntOrImpress(aPlayerStats.rouletteWinAmount, aPlayerStats.rouletteLossAmount, aGame.rouletteImpressWinAmt,
                       aGame.rouletteTauntLossAmt);
        if (ShouldShowInstructions(aGame, aPlayer.hasPlayedRoulette))
        {
            WriteLine(
                "Your main objective is to guess what color the ball will land on.");
            WriteLine("First, you'll have to pick what type of bet you want to make.");
            
        }
        
        WriteLine("Your options are:\n1. Straight\n2. Red/Black\n3. Odd/Even\n4. Column Bet");
        
        aPlayer.hasPlayedRoulette = true;
        
        char correctPick = ' ';
        int correctRow = 0;
        int landedCol = -1;
        
        bool isWinner;
        int landing = GetRandomNumber(aGame.ROULETTE_STRAIGHT_MIN, aGame.ROULETTE_STRAIGHT_MAX);
        int winAmount = landing == 0 ? bet * aRewards.rouletteZeroMultiplier : bet * aRewards.rouletteRewardMultiplier;;
        std::array<int, 12> correctRowArray;
        
        int playerDecision;
        std::cin >> playerDecision;

        while (std::cin.fail())
        {
            ClearInput();
            std::cin >> playerDecision;
        }
        
        playerDecision = Clamp(playerDecision, aGame.ROULETTE_BETTING_TYPE_MIN, aGame.ROULETTE_BETTING_TYPE_MAX);
        
        if (playerDecision == aGame.ROULETTE_STRAIGHT)
        {
            WriteLine("What's your guess? (0-36)");
            
            int playerGuess;
            std::cin >> playerGuess;

            while (std::cin.fail())
            {
                WriteLine("Please make a valid pick!");
                
                ClearInput();
                std::cin >> playerGuess;
            }
            
            playerGuess = Clamp(playerGuess, aGame.ROULETTE_STRAIGHT_MIN, aGame.ROULETTE_STRAIGHT_MAX);
            
            isWinner = playerGuess == landing;
        }
        else if (playerDecision == aGame.ROULETTE_RED_BLACK)
        {
            WriteLine("What's your guess? ('r' for red, 'b' for black)");
            
            char red = 'R';
            char black = 'B';
            
            bool isRedPicked = HasSubceded(GetRoll(), 50);
            
            correctPick = isRedPicked ? red : black;
            
            char playerGuess;
            std::cin >> playerGuess;
            
            // These two are in place to prevent the player from writing nonsense, ultimately preventing players from losing because of it (although I found it very funny)
            bool playerPickedRed = IsCharacter(playerGuess, red);
            bool playerPickedBlack = IsCharacter(playerGuess, black);
            
            bool isCorrectPick = IsCharacter(playerGuess, correctPick);

            while (!playerPickedRed && !playerPickedBlack)
            {
                WriteLine("Please make a valid pick!");
                
                while (std::cin.fail())
                {
                    ClearInput();
                    std::cin >> playerGuess;
                }
                
                std::cin >> playerGuess;
                
                playerPickedRed = IsCharacter(playerGuess, red);
                playerPickedBlack = IsCharacter(playerGuess, black);
            }
            
            isWinner = isCorrectPick;
        }
        else if (playerDecision == aGame.ROULETTE_ODD_EVEN)
        {
            char odd = 'O';
            char even = 'E';
            
            bool isLandingEven = IsEven(landing);
            
            WriteLine("What's your guess? ('e' for even, 'o' for odd)");
            
            char playerGuess;
            std::cin >> playerGuess;
            
            bool playerPickedEven = IsCharacter(playerGuess, even);
            bool playerPickedOdd = IsCharacter(playerGuess, odd);
            
            while (!playerPickedEven && !playerPickedOdd)
            {
                WriteLine("Please make a valid pick!");
                
                while (std::cin.fail())
                {
                    ClearInput();
                    std::cin >> playerGuess;
                }
                
                std::cin >> playerGuess;
                
                playerPickedEven = IsCharacter(playerGuess, even);
                playerPickedOdd = IsCharacter(playerGuess, odd);
            }
            
            isWinner = playerPickedEven && isLandingEven || playerPickedOdd && !isLandingEven;
        }
        else
        {
            int correctRowIndex = GetRandomNumber(1, 3);
            correctRowArray = BuildRow(correctRowIndex);
            correctRow = correctRowIndex;
            landedCol = GetRandomNumber(aGame.ROULETTE_ROW_MIN, aGame.ROULETTE_ROW_MAX);

            WriteLine("Which row are you picking? (1-3)");
            
            int playerGuess;
            std::cin >> playerGuess;
            
            while (std::cin.fail())
            {
                WriteLine("Please make a valid pick!");
                ClearInput();
                std::cin >> playerGuess;
            }
            
            playerGuess = Clamp(playerGuess, aGame.ROULETTE_ROW_MIN, aGame.ROULETTE_ROW_MAX);
            
            isWinner = playerGuess == correctRowIndex;
        }
        
        bool hasNumbers = playerDecision == aGame.ROULETTE_STRAIGHT || playerDecision == aGame.ROULETTE_ODD_EVEN;
        bool hasRow = playerDecision == aGame.ROULETTE_COLUMN_ROW;
      
        if (hasRow)
        {
            std::cout << (isWinner ? "Congratulations! It landed on": "Nice try! It landed on") << " " << correctRow << "(" << correctRowArray[landedCol] << ")" << '\n';
        }
        else if (hasNumbers)
        {
            std::cout << (isWinner ? "Well done! The ball landed on" : "Nice try! The ball landed on") << landing << '\n';
        }
        else
        {
            std::cout << "It landed on a " << correctPick << " rouge... so" << (isWinner ? " congratulations!" : " nice try! Better luck next time") << '\n';   
        }
        
        BroadcastWinOrLoss(aPlayer, aPlayerStats, isWinner, winAmount, bet, isWinner ? aPlayerStats.rouletteWinAmount : aPlayerStats.rouletteLossAmount);
    }
}
/*#include <iostream>
#include "Minigames.h"
#include "Kasino.h"
#include "Utilities.h"
#include <array>

// My code is highkey terrible ;-;
namespace GuessTheDiceSum
{
    void PlayGame(Casino& aCasino, EMinigame aMinigame)
    {
        if (HasExceeded(aCasino.playerStats.diceSumWinAmount, aCasino.game.diceSumWinLimit))
        {
            RefuseGame(aCasino.player.cantPlayDiceSum, aCasino);
            return;
        }

        int bet = GetBetAmount(aCasino);

        RollDice(aCasino);

        TauntOrImpress(aCasino.playerStats.diceSumWinAmount, aCasino.playerStats.diceSumLossAmount, aCasino.game.diceSumImpressWinAmt,
                       aCasino.game.diceSumTauntLossAmt);
        if (ShouldShowInstructions(aCasino, aCasino.player.hasPlayedDiceSum))
        {
            WriteLine(
                "Your only goal is to guess the sum of the dice. Your guess should not exceed 12 or fall behind 2!\nIf you do end exceeding or falling behind the boundary, we'll assume you mean 2 or 12.");
        }

        aCasino.player.hasPlayedDiceSum = true;
        WriteLine("Guess a number between 2-12:");
        int playerGuess;
        std::cin >> playerGuess;

        while (std::cin.fail())
        {
            ClearInput();
            std::cin >> playerGuess;
        }

        playerGuess = Clamp(playerGuess, aCasino.dice.diceSumMin, aCasino.dice.diceSumMax);

        BroadcastDiceResult(aCasino, true);

        bool isWinner = playerGuess == aCasino.dice.diceSum;

        int winAmount = bet * aCasino.rewards.guessTheSumRewardMultiplier;

        BroadcastWinOrLoss(aCasino, isWinner, winAmount, bet,
                           isWinner ? aCasino.playerStats.diceSumWinAmount : aCasino.playerStats.diceSumLossAmount);
    }
}

namespace OddOrEven
{
    void PlayGame(Casino& aCasino, EMinigame& aMinigame)
    {
        if (HasExceeded(aCasino.playerStats.oddEvenWinAmount, aCasino.game.oddEvenWinLimit))
        {
            RefuseGame(aCasino.player.cantPlayOddEven, aCasino);
            return;
        }


        int bet = GetBetAmount(aCasino);

        char even = 'E';
        char odd = 'O';

        RollDice(aCasino);

        TauntOrImpress(aCasino.playerStats.oddEvenWinAmount, aCasino.playerStats.oddEvenLossAmount, aCasino.game.oddEvenImpressWinAmt,
                       aCasino.game.oddEvenTauntLossAmt);

        if (ShouldShowInstructions(aCasino, aCasino.player.hasPlayedOddOrEven))
        {
            WriteLine(
                "Here you must guess if the dice are even or odd. If both dice aren't even or odd, the house wins.");
        }

        aCasino.player.hasPlayedOddOrEven = true;

        WriteLine("What are you guessing? Even or odd? Type 'e' for even or 'o' for odd!");

        char picked;
        std::cin >> picked;

        bool playerPickedEven = IsCharacter(picked, even);
        bool playerPickedOdd = IsCharacter(picked, odd);

        bool isEven = IsEven(aCasino.dice.die1) && IsEven(aCasino.dice.die2);
        bool isOdd = !IsEven(aCasino.dice.die1) && !IsEven(aCasino.dice.die2);

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

        BroadcastDiceResult(aCasino, false);

        int winAmount = bet * aCasino.rewards.oddOrEvenRewardMultiplier;

        BroadcastWinOrLoss(aCasino, isWinner, winAmount, bet,
                           isWinner ? aCasino.playerStats.oddEvenWinAmount : aCasino.playerStats.oddEvenLossAmount);
    }
}

namespace YesOrNo
{
    void PlayGame(Casino& aCasino, EMinigame& aMinigame)
    {
        if (HasExceeded(aCasino.playerStats.yesNoWinAmount, aCasino.game.yesNoWinLimit))
        {
            RefuseGame(aCasino.player.cantPlayYesNo, aCasino);
            return;
        }

        aCasino.rewards.yesNoRewardMultiplier = 1;

        // Store 'Y' and 'N' to prevent the code from being too repetitive
        char yes = 'Y';
        char no = 'N';

        int bet = GetBetAmount(aCasino);

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

        TauntOrImpress(aCasino.playerStats.yesNoWinAmount, aCasino.playerStats.yesNoLossAmount, aCasino.game.yesNoImpressWinAmt,
                       aCasino.game.yesNoTauntLossAmt);
        if (ShouldShowInstructions(aCasino, aCasino.player.hasPlayedYesOrNo))
        {
            WriteLine("Your sole objective is to guess whether the number picked is within the given range");
            WriteLine("Use 'y' if you think the number is in range and 'n' to guess if it is not.");
            std::cout << "Every time you have " << rewardMultiplierIncreaseThreshold <<
                " correct guesses in a row, your reward might increase!" << std::endl;
            WriteLine("** NOTE ** If you quit before guessing, you'll automatically lose.");
        }

        aCasino.player.hasPlayedYesOrNo = true;


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
                        aCasino.rewards.yesNoRewardMultiplier++;
                        std::cout << "Your reward multiplier has increased! It is now " << aCasino.rewards.
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

                    cachedReward += aCasino.rewards.yesNoBaseReward;
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
            winAmount = bet + (cachedReward * aCasino.rewards.yesNoRewardMultiplier);
        }

        bool isWinner = winAmount > 0 && isPlayerQuitting;

        BroadcastWinOrLoss(aCasino, isWinner, winAmount, bet,
                           isWinner ? aCasino.playerStats.yesNoWinAmount : aCasino.playerStats.yesNoLossAmount);
    }
}

namespace HigherOrLower
{
    void PlayGame(Casino& aCasino, EMinigame aMinigame)
    {
        if (HasExceeded(aCasino.playerStats.higherLowerWinAmount, aCasino.game.higherLowerWinLimit))
        {
            RefuseGame(aCasino.player.cantPlayHigherLower, aCasino);
            return;
        }

        int bet = GetBetAmount(aCasino);

        int pickedNumber = GetRoll();
        int nextPickedNumber = GetRoll();

        while (pickedNumber == nextPickedNumber)
        {
            pickedNumber = GetRoll();
        }

        TauntOrImpress(aCasino.playerStats.higherLowerWinAmount, aCasino.playerStats.higherLowerLossAmount,
                       aCasino.game.higherLowerImpressWinAmt,
                       aCasino.game.higherLowerTauntLossAmt);
        if (ShouldShowInstructions(aCasino, aCasino.player.hasPlayedHigherOrLower))
        {
            WriteLine(
                "The goal is simple: Guess whether or not the number visible on the screen is lower than the next one");
            WriteLine("Guess correctly and you'll get rewarded. However, a wrong guess will cost you everything!");
            WriteLine("(By quitting, you indirectly forfeit any possible winnings, thus the round becoming a loss)");
        }

        aCasino.player.hasPlayedHigherOrLower = true;

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
                    cachedReward += aCasino.rewards.higherLowerBaseReward;

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
            winAmount = bet + (cachedReward * aCasino.rewards.higherLowerRewardMultiplier);
        }

        bool isWinner = winAmount > 0 && isPlayerQuitting;

        BroadcastWinOrLoss(aCasino, isWinner, winAmount, bet,
                           isWinner ? aCasino.playerStats.higherLowerWinAmount : aCasino.playerStats.higherLowerLossAmount);
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
 #1#

// Betting Types:
// Straight
// Red/Black
// Odd/Even
// Column Bet
namespace Roulette
{
    /*
     * Goals for the next hand-in:
     * - Cleaner code!
     * - Splitting every one of these into separate classes.
     #1#
    void PlayGame(Casino& aCasino, EMinigame& aMinigame)
    {
        if (HasExceeded(aCasino.playerStats.rouletteWinAmount, aCasino.game.rouletteWinLimit))
        {
            RefuseGame(aCasino.player.cantPlayRoulette, aCasino);
            return;
        }

        int bet = GetBetAmount(aCasino);

        TauntOrImpress(aCasino.playerStats.rouletteWinAmount, aCasino.playerStats.rouletteLossAmount, aCasino.game.rouletteImpressWinAmt,
                       aCasino.game.rouletteTauntLossAmt);
        if (ShouldShowInstructions(aCasino, aCasino.player.hasPlayedRoulette))
        {
            WriteLine(
                "Your main objective is to guess what color the ball will land on.");
            WriteLine("First, you'll have to pick what type of bet you want to make.\n");

            WriteLine("** STRAIGHT **: Requires you to bet on a rouge. You're only able to guess 0-36");
            WriteLine("** RED/BLACK **: Picking this will require you to pick either red or black");
            WriteLine("** ODD/EVEN **: You'll need to guess either odd or even.");
            WriteLine("** COLUMN BET **: Allows you to bet from 0-3. In this case, zero is standalone.\n");

            WriteLine(
                "If you guess correctly, your pay out stays the same. However, betting on 0 will give you 36x the outcome!");
        }

        WriteLine("Your options are:\n1. Straight\n2. Red/Black\n3. Odd/Even\n4. Column Bet");

        aCasino.player.hasPlayedRoulette = true;

        char correctPick = ' ';
        int correctRow = 0;
        int landedCol = -1;

        bool isWinner;
        int landing = GetRandomNumber(aCasino.game.ROULETTE_STRAIGHT_MIN, aCasino.game.ROULETTE_STRAIGHT_MAX);
        int winAmount = landing == 0 ? bet * aCasino.rewards.rouletteZeroMultiplier : bet * aCasino.rewards.rouletteRewardMultiplier;;
        std::array<int, 12> correctRowArray;

        int playerDecision;
        std::cin >> playerDecision;

        while (std::cin.fail())
        {
            ClearInput();
            std::cin >> playerDecision;
        }

        playerDecision = Clamp(playerDecision, aCasino.game.ROULETTE_BETTING_TYPE_MIN, aCasino.game.ROULETTE_BETTING_TYPE_MAX);

        if (playerDecision == aCasino.game.ROULETTE_STRAIGHT)
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

            playerGuess = Clamp(playerGuess, aCasino.game.ROULETTE_STRAIGHT_MIN, aCasino.game.ROULETTE_STRAIGHT_MAX);

            isWinner = playerGuess == landing;
        }
        else if (playerDecision == aCasino.game.ROULETTE_RED_BLACK)
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
        else if (playerDecision == aCasino.game.ROULETTE_ODD_EVEN)
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
            int correctRowIndex = GetRandomNumber(0, 3);
            correctRowArray = BuildRow(correctRowIndex);
            correctRow = correctRowIndex;
            landedCol = GetRandomNumber(aCasino.game.ROULETTE_ROW_MIN, aCasino.game.ROULETTE_ROW_MAX);

            WriteLine("Which row are you picking? (0-3)");

            int playerGuess;
            std::cin >> playerGuess;

            while (std::cin.fail())
            {
                WriteLine("Please make a valid pick!");
                ClearInput();
                std::cin >> playerGuess;
            }

            playerGuess = Clamp(playerGuess, 0, aCasino.game.ROULETTE_ROW_MAX);

            winAmount = correctRow == 0
                            ? bet * aCasino.rewards.rouletteZeroMultiplier
                            : bet * aCasino.rewards.rouletteRewardMultiplier;
            isWinner = playerGuess == correctRowIndex;
        }

        bool hasNumbers = playerDecision == aCasino.game.ROULETTE_STRAIGHT || playerDecision == aCasino.game.ROULETTE_ODD_EVEN;
        bool hasRow = playerDecision == aCasino.game.ROULETTE_COLUMN_ROW;

        if (hasRow)
        {
            bool isAboveZero = correctRow > 0;

            std::cout << (isWinner ? "Congratulations! It landed on" : "Nice try! It landed on") << " " << correctRow;
            if (isAboveZero) std::cout << "(" << (isAboveZero ? correctRowArray[landedCol] : 0) << ")" << '\n';
            else std::cout << '\n';
        }
        else if (hasNumbers)
        {
            std::cout << (isWinner ? "Well done! The ball landed on" : "Nice try! The ball landed on") << " " << landing <<
                '\n';
        }
        else
        {
            std::cout << "It landed on a " << correctPick << " rouge... so" << (isWinner
                ? " congratulations!"
                : " nice try! Better luck next time") << '\n';
        }

        BroadcastWinOrLoss(aCasino, isWinner, winAmount, bet,
                           isWinner ? aCasino.playerStats.rouletteWinAmount : aCasino.playerStats.rouletteLossAmount);
    }
}*/
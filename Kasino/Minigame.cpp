#include "Minigame.h"
#include "Utilities.h"
#include <iostream>

void Minigame::Initialize(EMinigameType& aMinigame)
{
    // The win limit index will be gone once subclasses are involved!
    switch (aMinigame)
    {
    case EMinigameType::GuessTheDiceSum:
        myRewardMultiplier = 5;
        myWinLimit = 2500;
        myGameIndex = MINIGAME_GUESS_THE_DICE_SUM;
        break;
    case EMinigameType::OddOrEven:
        myRewardMultiplier = 4;
        myWinLimit = 1500;
        myGameIndex = MINIGAME_ODD_OR_EVEN;
        break;
    case EMinigameType::YesOrNo:
        myBaseReward = 25;
        myUseCachedReward = true;
        myRewardIncreaseThreshold = 3;
        myWinLimit = 4000;
        myGameIndex = MINIGAME_YES_OR_NO;
        myPlayerCanQuit = true;
        myIsForcedVictory = true;
        break;
    case EMinigameType::HigherOrLower:
        myRewardMultiplier = 2;
        myBaseReward = 50;
        myUseCachedReward = true;
        myWinLimit = 3500;
        myGameIndex = MINIGAME_HIGHER_OR_LOWER;
        myPlayerCanQuit = true;
        myIsForcedVictory = true;
        break;
    case EMinigameType::Roulette:
        myRewardMultiplier = 3;
        myAlternativeRewardMultiplier = 36;
        myWinLimit = 3000;
        myGameIndex = MINIGAME_ROULETTE;
        myIsForcedWinAmount = false;
        myAlternativeRewardMultiplier = 36;
        break;
    }
    
    myMinigameType = aMinigame;
}

const char *Minigame::FromMinigameToChar(EMinigameType& aMinigame)
{
    switch (aMinigame)
    {
    case EMinigameType::GuessTheDiceSum:
        return "Guess The Dice Sum";
    case EMinigameType::OddOrEven:
        return "Odd or Even";
    case EMinigameType::YesOrNo:
        return "Yes or No";
    case EMinigameType::HigherOrLower:
        return "Higher or Lower";
    case EMinigameType::Roulette:
        return "Roulette";
    }
    
    return "";
}

void Minigame::PlayGame(Casino& aCasino)
{
    if (HasExceeded(myWinAmount, myWinLimit))
    {
        RefuseGame(myCantPlay, aCasino);
        return;
    }
    
    myCachedReward = 0;
    
    myBet = GetBetAmount(aCasino);
    TauntOrImpress(myWinAmount, myLossAmount, myWinImpressAmount,
                       myLossTauntAmount);


    if (aCasino.shouldShowInstructions[myGameIndex])
    {
        ShowInstructions();
    }
    
    aCasino.shouldShowInstructions[myGameIndex] = false;
    
    myRangeStart = GetRoll();
    myRangeEnd = GetRoll();
    
    if (myRangeStart > myRangeEnd)
    {
        int temp = myRangeEnd;
        myRangeStart = myRangeEnd;
        myRangeEnd = temp;
    }

    if (myRangeStart == myRangeEnd)
    {
        myRangeEnd++;
    }
    
    RollDice(aCasino);
    
    switch (myMinigameType)
    {
    case EMinigameType::GuessTheDiceSum:
        WriteLine("Guess a number (1-12)");
        break;
    case EMinigameType::OddOrEven:
        WriteLine("What are you guessing? Even or odd? Type 'e' for even or 'o' for odd!");
        break;
    case EMinigameType::YesOrNo:
        std::cout << "Is the number between " << myRangeStart << " and " << myRangeEnd << "?" << '\n';
        break;
    case EMinigameType::HigherOrLower:
        std::cout << "Do you think " << myRangeStart << " is higher or lower than the next number?" << '\n';
        break;
    case EMinigameType::Roulette:
        WriteLine("What do you feel like playing?\n1. Straight\n2. Red/Black\n3. Odd/even\n4. Column Bet");
        break;
    }
    
    OnPlay(aCasino);
}

void Minigame::Reset()
{
    myWinAmount = 0;
    myLossAmount = 0;
    myCantPlay = false;
}

void Minigame::ShowInstructions()
{
    switch (myMinigameType)
    {
    case EMinigameType::GuessTheDiceSum:
        WriteLine(
                "Your only goal is to guess the sum of the dice. Your guess should not exceed 12 or fall behind 2!\nIf you do end exceeding or falling behind the boundary, we'll assume you mean 2 or 12.");
        break;
    case EMinigameType::OddOrEven:
        WriteLine(
                "Here you must guess if the dice are even or odd. If both dice aren't even or odd, the house wins.");
        break;
    case EMinigameType::YesOrNo:
        WriteLine("Your sole objective is to guess whether the number picked is within the given range");
        WriteLine("Use 'y' if you think the number is in range and 'n' to guess if it is not.");
        std::cout << "Every time you have " << myRewardIncreaseThreshold <<
            " consecutive correct guesses in a row, your reward might increase!" << '\n';
        WriteLine("** NOTE ** If you quit before guessing, you'll automatically lose.");
        break;
    case EMinigameType::HigherOrLower:
        WriteLine(
                "The goal is simple: Guess whether or not the number visible on the screen is lower than the next one");
        WriteLine("Guess correctly and you'll get rewarded. However, a wrong guess will cost you everything!");
        WriteLine("(By quitting, you indirectly forfeit any possible winnings, thus the round becoming a loss)");
        break;
    case EMinigameType::Roulette:
        WriteLine(
                "Your main objective is to guess what color the ball will land on.");
        WriteLine("First, you'll have to pick what type of bet you want to make.\n");

        WriteLine("** STRAIGHT **: Requires you to bet on a rouge. You're only able to guess 0-36");
        WriteLine("** RED/BLACK **: Picking this will require you to pick either red or black");
        WriteLine("** ODD/EVEN **: You'll need to guess either odd or even.");
        WriteLine("** COLUMN BET **: Allows you to bet from 0-3. In this case, zero is standalone.\n");

        WriteLine(
            "If you guess correctly, your pay out stays the same. However, betting on 0 will give you 36x the outcome!");
        break;
    }
}

void Minigame::OnPlay(Casino& aCasino)
{
    bool isWinner = false;
    int winAmount = 0;
    
    switch (myMinigameType)
    {
    case EMinigameType::GuessTheDiceSum:
        int playerGuess;
        std::cin >> playerGuess;

        while (std::cin.fail())
        {
            ClearInput();
            std::cin >> playerGuess;
        }

        playerGuess = Clamp(playerGuess, aCasino.dice.diceSumMin, aCasino.dice.diceSumMax);

        BroadcastDiceResult(aCasino, true);
        isWinner = playerGuess == aCasino.dice.diceSum;
        break;
    case EMinigameType::OddOrEven:
        {
            myFirstChoice = 'E';
            mySecondChoice = 'O';
            
            char picked;
            std::cin >> picked;

            myHasPlayerPickedFirst = IsCharacter(picked, myFirstChoice);
            myHasPlayerPickedSecond = IsCharacter(picked, mySecondChoice);

            bool isEven = IsEven(aCasino.dice.die1) && IsEven(aCasino.dice.die2);
            bool isOdd = !IsEven(aCasino.dice.die1) && !IsEven(aCasino.dice.die2);

            while (!myHasPlayerPickedFirst && !myHasPlayerPickedSecond)
            {
                while (std::cin.fail())
                {
                    ClearInput();
                    std::cin >> picked;
                }

                std::cin >> picked;
                myHasPlayerPickedFirst = IsCharacter(picked, myFirstChoice);
                myHasPlayerPickedSecond = IsCharacter(picked, mySecondChoice);
            }

            isWinner = (myHasPlayerPickedFirst && isEven) || (myHasPlayerPickedSecond && isOdd);

            BroadcastDiceResult(aCasino, false);
        }
        break;
    case EMinigameType::YesOrNo:
        {
            myRewardMultiplier = 1;
            
            myFirstChoice = 'Y';
            mySecondChoice = 'N';
        
            int actualRoll = GetRoll();
        
            int numConsecutiveCorrectGuesses = 0;
        
            char playerInput;
            std::cin >> playerInput;
            myHasPlayerPickedFirst = IsCharacter(playerInput, myFirstChoice);
            myHasPlayerPickedSecond = IsCharacter(playerInput, mySecondChoice);

            myIsPlayerQuitting = IsCharacter(playerInput, 'Q');
            
            while (!myIsPlayerQuitting)
            {
                while (std::cin.fail())
                {
                    ClearInput();
                    std::cin >> playerInput;
                }

                if (myHasPlayerPickedFirst || myHasPlayerPickedSecond)
                {
                    if (myHasPlayerPickedSecond && !IsInRange(actualRoll, myRangeStart, myRangeEnd) || myHasPlayerPickedFirst && IsInRange(
                        actualRoll, myRangeStart, myRangeEnd))
                    {
                        numConsecutiveCorrectGuesses++;
                        if (numConsecutiveCorrectGuesses >= myRewardIncreaseThreshold)
                        {
                            numConsecutiveCorrectGuesses = 0;
                            myRewardMultiplier++;
                            std::cout << "Your reward multiplier has increased! It is now " << myRewardMultiplier << "!" << '\n';
                        }

                        actualRoll = GetRoll();
                        myRangeStart = GetRoll();
                        myRangeEnd = GetRoll();
                        
                        if (myRangeStart > myRangeEnd)
                        {
                            int temp = myRangeEnd;
                            myRangeStart = myRangeEnd;
                            myRangeEnd = temp;
                        }

                        if (myRangeStart == myRangeEnd)
                        {
                            myRangeEnd++;
                        }

                        std::cout << "Is the number between " << myRangeStart << " and " << myRangeEnd << "?" << '\n';

                        myCachedReward += myBaseReward;
                    }
                    else
                    {
                        myCachedReward = 0; // Reset the cached reward if the player guesses wrong
                        std::cout << "You guessed wrong! The number was " << actualRoll << " which means it was " << (
                            IsInRange(actualRoll, myRangeStart, myRangeEnd) ? "in" : "not in") << " the range." << '\n';
                        break;
                    }
                }

                std::cin >> playerInput;

                myHasPlayerPickedFirst = IsCharacter(playerInput, myFirstChoice);
                myHasPlayerPickedSecond = IsCharacter(playerInput, mySecondChoice);
                myIsPlayerQuitting = IsCharacter(playerInput, 'q');
            }
        }
        
        break;
    case EMinigameType::HigherOrLower:
        {
            bool isHigher = myRangeStart > myRangeEnd;
                
            myFirstChoice = 'H';
            mySecondChoice = 'L';
                
            char playerPick;
            std::cin >> playerPick;

            myHasPlayerPickedFirst = IsCharacter(playerPick, myFirstChoice);
            myHasPlayerPickedSecond = IsCharacter(playerPick, mySecondChoice);

            myIsPlayerQuitting = IsCharacter(playerPick, myQuitChoice);

            while (!myIsPlayerQuitting)
            {
                while (std::cin.fail())
                {
                    ClearInput();
                    std::cin >> playerGuess;
                }

                if (myHasPlayerPickedFirst || myHasPlayerPickedSecond)
                {
                    if (myHasPlayerPickedFirst && isHigher || myHasPlayerPickedSecond && !isHigher)
                    {
                        // It's flipped hehe
                        std::cout << "That's correct! " << myRangeStart << " is " << (isHigher ? "higher" : "lower") <<
                            " than " << myRangeEnd << "!" << "\n";
                        myCachedReward += myBaseReward;

                        int temp = myRangeEnd;
                        myRangeStart = temp;
                        myRangeEnd = GetRoll();

                        while (myRangeStart == myRangeEnd)
                        {
                            myRangeStart = GetRoll();
                        }

                        isHigher = myRangeStart > myRangeEnd;

                        std::cout << "Do you think " << myRangeStart << " is higher or lower than the next number?" << '\n';
                    }
                    else
                    {
                        std::cout << "The number after " << myRangeStart << " was " << myRangeEnd << "!\n";
                        WriteLine("Better luck next time!");
                        myCachedReward = 0;
                        break;
                    }
                }

                std::cin >> playerPick;
                myHasPlayerPickedFirst = IsCharacter(playerPick, myFirstChoice);
                myHasPlayerPickedSecond = IsCharacter(playerPick, mySecondChoice);
                myIsPlayerQuitting = IsCharacter(playerPick, 'Q');
            }
        }
        break;
    case EMinigameType::Roulette:
        {
            char correctPick = ' ';
            int correctRow = 0;
            int landedCol = -1;

            int landing = GetRandomNumber(aCasino.game.ROULETTE_STRAIGHT_MIN, aCasino.game.ROULETTE_STRAIGHT_MAX);
            winAmount = landing == 0 ? myBet * myAlternativeRewardMultiplier : myBet * myRewardMultiplier;
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

                int playerInput;
                std::cin >> playerInput;

                while (std::cin.fail())
                {
                    WriteLine("Please make a valid pick!");

                    ClearInput();
                    std::cin >> playerInput;
                }

                playerInput = Clamp(playerInput, aCasino.game.ROULETTE_STRAIGHT_MIN, aCasino.game.ROULETTE_STRAIGHT_MAX);

                isWinner = playerInput == landing;
            }
            else if (playerDecision == aCasino.game.ROULETTE_RED_BLACK)
            {
                WriteLine("What's your guess? ('r' for red, 'b' for black)");

                myFirstChoice = 'R';
                mySecondChoice = 'B';

                int cachedRoll = GetRoll();
                bool isRedPicked = HasSubceded(cachedRoll, 50) && cachedRoll > 0;

                correctPick = isRedPicked ? myFirstChoice : mySecondChoice;

                char playerPick;
                std::cin >> playerPick;

                // These two are in place to prevent the player from writing nonsense, ultimately preventing players from losing because of it (although I found it very funny)
                myHasPlayerPickedFirst = IsCharacter(playerPick, myFirstChoice);
                myHasPlayerPickedSecond = IsCharacter(playerPick, mySecondChoice);

                bool isCorrectPick = IsCharacter(playerPick, correctPick);

                while (!myHasPlayerPickedFirst && !myHasPlayerPickedSecond)
                {
                    WriteLine("Please make a valid pick!");

                    while (std::cin.fail())
                    {
                        ClearInput();
                        std::cin >> playerPick;
                    }

                    std::cin >> playerPick;

                    myHasPlayerPickedFirst = IsCharacter(playerPick, myFirstChoice);
                    myHasPlayerPickedSecond = IsCharacter(playerPick, mySecondChoice);
                }

                isWinner = isCorrectPick;
            }
            else if (playerDecision == aCasino.game.ROULETTE_ODD_EVEN)
            {
                myFirstChoice = 'O';
                mySecondChoice = 'E';

                bool isLandingEven = IsEven(landing);

                WriteLine("What's your guess? ('e' for even, 'o' for odd)");

                char playerPick;
                std::cin >> playerPick;

                myHasPlayerPickedFirst = IsCharacter(playerPick, myFirstChoice);
                myHasPlayerPickedSecond = IsCharacter(playerPick, mySecondChoice);

                while (!myHasPlayerPickedFirst && !myHasPlayerPickedSecond)
                {
                    WriteLine("Please make a valid pick!");

                    while (std::cin.fail())
                    {
                        ClearInput();
                        std::cin >> playerPick;
                    }

                    std::cin >> playerPick;

                    myHasPlayerPickedFirst = IsCharacter(playerPick, myFirstChoice);
                    myHasPlayerPickedSecond = IsCharacter(playerPick, mySecondChoice);
                }

                isWinner = myHasPlayerPickedFirst && !isLandingEven || myHasPlayerPickedSecond && isLandingEven;
            }
            else
            {
                int correctRowIndex = GetRandomNumber(0, 3);
                correctRowArray = BuildRow(correctRowIndex);
                correctRow = correctRowIndex;
                landedCol = GetRandomNumber(aCasino.game.ROULETTE_ROW_MIN, aCasino.game.ROULETTE_ROW_MAX);

                WriteLine("Which row are you picking? (0-3)");

                int playerRowPick;
                std::cin >> playerRowPick;

                while (std::cin.fail())
                {
                    WriteLine("Please make a valid pick!");
                    ClearInput();
                    std::cin >> playerRowPick;
                }

                playerRowPick = Clamp(playerRowPick, 0, aCasino.game.ROULETTE_ROW_MAX);

                winAmount = correctRow == 0
                                ? myBet * myAlternativeRewardMultiplier
                                : myBet * myRewardMultiplier;
                isWinner = playerRowPick == correctRowIndex;
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
        }
        break;
    }
    
    if (myIsForcedWinAmount) winAmount = (myUseCachedReward ? myBet + (myCachedReward * myRewardMultiplier) : myBet * myRewardMultiplier);
    bool hasWonMoney = winAmount > 0;
    
    if (myIsForcedVictory) isWinner = (!myPlayerCanQuit ? hasWonMoney : hasWonMoney && myIsPlayerQuitting);
    
    BroadcastWinOrLoss(aCasino, isWinner, winAmount, myBet,
                           isWinner ? myWinAmount : myLossAmount);
}
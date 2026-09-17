#include "Helpers.h"
#include "Kasino.h"
#include "StateController.h"

#include "Utilities.h"

/*
 * This is technically not a state machine. Instead, it is a state controller.
 * It is more limited version than a state machine. 
*/
void Pick(int aChoice, bool aIsInGame, Casino& aCasino)
{
    ClearConsole();
    if (aIsInGame)
    {
        aCasino.player.pickedMinigame = aChoice;

        switch (aChoice)
        {
        case 1:
            SwitchTo(EOptions::GuessTheDiceSum, aCasino);
            break;
        case 2:
            SwitchTo(EOptions::OddOrEven, aCasino);
            break;
        case 3:
            SwitchTo(EOptions::YesOrNo, aCasino);
            break;
        case 4:
            SwitchTo(EOptions::HigherOrLower, aCasino);
            break;
        case 5:
            SwitchTo(EOptions::Roulette, aCasino);
            break;
        case 6:
            SwitchTo(EOptions::CashOut, aCasino);
            break;
        case 7:
            ChangeState(EStates::MainMenu, aCasino);
            break;
        }
        return;
    }

    switch (aChoice)
    {
    case 1:
        ChangeState(EStates::Game, aCasino);
        break;
    case 2:
        SwitchTo(EOptions::About, aCasino);
        break;
    case 3:
        SwitchTo(EOptions::Stats, aCasino);
        break;
    case 4:
        SwitchTo(EOptions::Quit, aCasino);
        break;
    }
}

void ChangeState(EStates aTargetState, Casino& aCasino)
{
    switch (aTargetState)
    {
    case EStates::MainMenu:
        EnterMainMenu(aCasino);
        break;
    case EStates::Game:
        EnterGamePicker(aCasino);
        break;
    }
}

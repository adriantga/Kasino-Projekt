#include "Helpers.h"
#include "Kasino.h"
#include "StateController.h"

#include "Utilities.h"

/*
 * This is technically not a state machine. Instead, it is a state controller.
 * It is more limited version than a state machine. 
*/
void Pick(int aChoice, bool aIsInGame, Player& aPlayer)
{
    ClearConsole();
    if (aIsInGame)
    {
        aPlayer.pickedMinigame = aChoice;

        switch (aChoice)
        {
        case 1:
            SwitchTo(EOptions::GuessTheDiceSum);
            break;
        case 2:
            SwitchTo(EOptions::OddOrEven);
            break;
        case 3:
            SwitchTo(EOptions::YesOrNo);
            break;
        case 4:
            SwitchTo(EOptions::CashOut);
            break;
        case 5:
            ChangeState(EStates::MainMenu);
            break;
        }
        return;
    }

    switch (aChoice)
    {
    case 1:
        ChangeState(EStates::Game);
        break;
    case 2:
        SwitchTo(EOptions::About);
        break;
    case 3:
        SwitchTo(EOptions::Stats);
        break;
    case 4:
        SwitchTo(EOptions::Quit);
        break;
    }
}

void ChangeState(EStates aTargetState)
{
    switch (aTargetState)
    {
    case EStates::MainMenu:
        EnterMainMenu();
        break;
    case EStates::Game:
        EnterGamePicker();
        break;
    }
}

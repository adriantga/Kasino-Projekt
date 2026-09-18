#include "Helpers.h"
#include "Kasino.h"
#include "StateController.h"

#include <iostream>

#include "Minigame.h"
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
        
        if (aChoice > aCasino.minigames.size())
        {
            if (aChoice == aCasino.minigames.size() + 1)
            {
                SwitchTo(EOptions::CashOut, aCasino);
            }
            else
            {
                ChangeState(EStates::MainMenu, aCasino);
            }
        }
        else
        {
            aCasino.minigames[aChoice - 1].PlayGame(aCasino);
        }
        
        // A sin has been committed.
        aChoice = Clamp(aChoice, 1, int(aCasino.minigames.size() + 2));
        
        
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

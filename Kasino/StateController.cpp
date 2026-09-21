#include "Helpers.h"
#include "Kasino.h"
#include "StateController.h"

#include "Minigame.h"
#include "Utilities.h"

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
        if (aCasino.player.hasEnteredName)
        {
            ChangeState(EStates::Game, aCasino);
        }
        else
        {
            EnterNamePicker(aCasino);
        }
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
    case EStates::Game:
        EnterGamePicker(aCasino);
        break;
    case EStates::MainMenu:
        EnterMainMenu(aCasino);
        break;
    }
}

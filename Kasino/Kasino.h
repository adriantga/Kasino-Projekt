#pragma once

void EnterGamePicker();
void ResetBalance();
void Exit();
void BroadcastPlayerBalance(bool aStylize);
void AddBalance(int aAmount, int& aMinigame);
void RemoveBalance(int aAmount, int& aMinigame);
void EnterMainMenu();
void GameOver();
bool AskPlayerAgain(bool aIsInGame);
void RefuseGame(bool& aCantPlay);
void TauntOrImpress(int aWinAmount, int aLossAmount, int aImpressWinAmt, int aTauntLossAmt);
void SwitchTo(EOptions aOption);

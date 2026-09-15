#pragma once

void EnterGamePicker();
void ResetBalance();
void Exit();
void BroadcastPlayerBalance(bool aStylize);
void AddBalance(int aAmount, int& aMinigame);
void RemoveBalance(int aAmount, int& aMinigame);
void EnterMainMenu();
void Pick(int aChoice, bool isInGame);
int GetBetAmount();
void GameOver();
bool AskPlayerAgain(bool aIsInGame);
void RefuseGame(bool& aCantPlay);
void TauntOrImpress(int aWinAmount, int aLossAmount, int aImpressWinAmt, int aTauntLossAmt);
void BroadcastDiceResult(bool aShowSum);
void BroadcastWinOrLoss(bool aIsWinner, int aWinAmount, int aLoseAmount, int& aMinigame);
void SwitchTo(EOptions aOption);
void ChangeState(EStates aTargetState);
void DrawMenu(int& input, const char aTitleText[], const char aOptions[], int aNumOptions = 3, const char aExtraOptions[] = "");
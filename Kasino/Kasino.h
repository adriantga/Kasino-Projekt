#pragma once
#include "Helpers.h"

struct Dice;
struct Player;
struct PlayerStats;
struct Game;
struct Rewards;
struct Constants;

void EnterGamePicker(Casino& aCasino);
void ResetBalance(Casino& aCasino);
void About(Casino& aCasino);
void Exit(Casino& aCasino);
void CashOut(Casino& aCasino);
void BroadcastPlayerBalance(bool aStylize, Casino& aCasino);
void AddBalance(int aAmount, int& aMinigame, Casino& aCasino);
void RemoveBalance(int aAmount, int& aMinigame, Casino& aCasino);
void EnterMainMenu(Casino& aCasino);
void GameOver(Casino& aCasino);
bool AskPlayerAgain(bool aIsInGame, Casino& aCasino);
void RefuseGame(bool& aCantPlay, Casino& aCasino);
void TauntOrImpress(int aWinAmount, int aLossAmount, int aImpressWinAmt, int aTauntLossAmt);
void SwitchTo(EOptions aOption, Casino& casino);

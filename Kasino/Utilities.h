#pragma once

bool IsCharacter(char aInput, char aValue);
int GetRandomNumber(int aMin, int aMax);
void ClearConsole();
void ClearInput();
void Pause();
int RollDie();
void RollDice(Dice& aDice);
int Min(int aValue, int aMin);
int Max(int aValue, int aMax);
int Clamp(int aValue, int aMin, int aMax);
bool IsInRange(int aValue, int aMin, int aMax);
bool HasSubceded(int aCurrent, int aTarget);
bool HasExceeded(int aCurrent, int aTarget);
bool IsEven(int aValue);
int GetRoll();
void DrawBreakerLine();
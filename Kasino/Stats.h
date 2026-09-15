#pragma once

#include "Helpers.h"

void ShowStats(PlayerStats& aStats);
void ResetStats(PlayerStats& aStats);
void SetResult(PlayerStats& aStats, int aMatchIndex, bool aIsWin);
void AddPlayedGame(PlayerStats& aStats, bool aIsWinner);

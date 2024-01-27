#pragma once

#include "CoreMinimal.h"

class GLOBAL_2024_API RoundManager
{
private:

	void StartNewRound();
	void ApplyRoundResult(int winnerPlayerIndex);
	void FinishCombat();

public:
	RoundManager();
	~RoundManager();
};

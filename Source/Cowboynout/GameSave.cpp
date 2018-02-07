// you no take candle!

#include "GameSave.h"

UGameSave::UGameSave() 
{
	SaveSlotName = TEXT("localHighScore");
	UserIndex = 0;

	Score = 0;
	Timer = 0;
}


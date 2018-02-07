// you no take candle!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameSave.generated.h"

/**
 * 
 */
UCLASS()
class COWBOYNOUT_API UGameSave : public USaveGame
{
	GENERATED_BODY()
	
public:
	
	UGameSave();
	
	UPROPERTY(VisibleAnywhere, Category = Basic)
	int Score;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	float Timer;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;


	
};

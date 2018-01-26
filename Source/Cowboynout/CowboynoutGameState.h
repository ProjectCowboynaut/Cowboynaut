// you no take candle!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CowboynoutGameState.generated.h"

/**
 * 
 */
UCLASS()
class COWBOYNOUT_API ACowboynoutGameState : public AGameStateBase
{
	GENERATED_BODY()


public:
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Character")
	bool isPaused;
	
	
};

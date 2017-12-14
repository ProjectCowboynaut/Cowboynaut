// you no take candle!

#pragma once

#include "GameFramework/Actor.h"

#include "Splosion.generated.h"

UCLASS()
class COWBOYNOUT_API ASplosion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplosion();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	float nadeTimer;
	
	UPROPERTY()
	float nadeTimerFull;

	UPROPERTY()
	bool theEnd;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};

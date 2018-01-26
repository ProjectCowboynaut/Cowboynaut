// you no take candle!

#pragma once

#include "Engine.h"
#include "Cowboynout.h"
#include "Components/ActorComponent.h"
#include "Enemy.h"
#include "BossComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COWBOYNOUT_API UBossComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossComponent();

	UFUNCTION()
	void BossFight();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};

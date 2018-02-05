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

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void BossFight();

	UFUNCTION(BlueprintCallable)
	void SpawnBullets(TSubclassOf<AProjectile> bulletBP, FVector center, float radius, int numberOfBulletsToFire, float bulletSpeed, float bulletDamage, float DeltaTime);

	// number of phases to go through, b4 starting at 0 again
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int numberOfPhases;
	
	// number of the actual running phase
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int numberOfPhasesLive;

	// max timer per phase
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float phaseTimerMax;

	// running timer
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float phaseTimerLive;

	UPROPERTY()
	float rotationTicker;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;		
	
};

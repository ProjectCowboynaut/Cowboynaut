// you no take candle!

#pragma once

#include "Engine.h"
#include "Cowboynout.h"
#include "Components/ActorComponent.h"
#include "Enemy.h"
#include "BossComponent.generated.h"

UENUM(Blueprintable)
enum class BossState : uint8
{
	BossIdle UMETA(DisplayName = "Boss Idling"),
	BossAttack UMETA(DisplayName = "Boss Attacking /w Shield deactivated"),
	BossShield UMETA(DisplayName = "Boss Spawning w/ Shield activated")
};

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
	void BossFight(float DeltaTime);

	UFUNCTION()
	void SwitchState();

	UFUNCTION(BlueprintCallable)
	void SpawnBullets(TSubclassOf<AProjectile> bulletBP, FVector center, float radius, int numberOfBulletsToFire, float bulletSpeed, float bulletDamage, float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEnemy> DroneBP;

	// idle, attack, spawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	BossState bossState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int stateSwitchesCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float stateSwitchPercentage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float droneSpawnTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float droneSpawnTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AEnemy* boss;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int numberOfTotalStateSwitches;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int numberOfDronesToSpawnPerPhase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int numberOfDronesSpawned;

	// number of phases to go through, b4 starting at 0 again  (different bullet during attack state)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int numberOfPhases;
	
	// number of the actual running phase
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int numberOfPhasesLive;

	// max timer per phase
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float phaseTimerMax;

	// running timer
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float phaseTimerLive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float healthForNextPhase;

	UPROPERTY()
	float rotationTicker;

protected:
	virtual void BeginPlay() override;
};

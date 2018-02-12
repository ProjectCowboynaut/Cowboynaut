// you no take candle!

#pragma once

#include "Engine.h"
#include "Cowboynout.h"
#include "Components/ActorComponent.h"
#include "Enemy.h"
#include "CowboynoutGameMode.h"
#include "EnemySpawner.h"
#include "BossComponent.generated.h"

UENUM(Blueprintable)
enum class BossState : uint8
{
	BossIdle UMETA(DisplayName = "Boss Idling"),
	BossAttack UMETA(DisplayName = "Boss Attacking /w Shield deactivated"),
	BossShield UMETA(DisplayName = "Boss Spawning w/ Shield activated")
};


USTRUCT(BlueprintType)
struct FAttackPattern
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AProjectile> bulletBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int numberOfBulletsToFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float bulletSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float bulletDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float rotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float bulletLifeTime;
};


USTRUCT(BlueprintType)
struct FStages
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	BossState stageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int dronesToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float spawnFrequency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float healthPercentageToSwitchStage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAttackPattern> attackPatterns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AEnemySpawner*> spawnerList;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COWBOYNOUT_API UBossComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector bossSpawnLocation;

	UPROPERTY(VisibleAnywhere, Category = "Level Stuff")
	TArray<AActor*> foundActors;

	UPROPERTY(VisibleAnywhere, Category = "Level Stuff")
	TArray<AActor*> bossDrones;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float bossHealthMax;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int phaseCtr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float lastShotFired;

	// how often should the boss shot in attack phases
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float shotTimer;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void BossFight(float DeltaTime);

	UPROPERTY()
	TArray<int32> bossStages;

	UFUNCTION()
	void SwitchState(BossState state);

	UFUNCTION(BlueprintCallable)
	void SpawnBullets(TSubclassOf<AProjectile> bulletBP, float radius, int numberOfBulletsToFire, float bulletSpeed, float bulletDamage, float deltaTime, float attackRate, float bulletLifeTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEnemy> DroneBP;

	// idle, attack, spawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	BossState bossState;

	// what kind of stage is the boss in
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	BossState actualStageType;

	/*	[phase nr (int x)] [phase type (int x)] [attacks to use (int x,int y,…)] [drone number to spawn (int x)] [health to switch (int x)]
		>>  0 1 1,1,4,1,2 10 8000
	*/

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
	TArray<FStages> stages;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int stateSwitchesCount;

	// percentage of health to switch into next state
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float stateSwitchPercentage;

	// how long should drones be spawned (or ammount below)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float droneSpawnTimer;

	// how many drones should be spawned (or time above)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float droneSpawnTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AEnemy* boss;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int numberOfTotalStateSwitches;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
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

	// only displaying the set value for the actual stages, set in stages
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float healthForNextStage;

	UPROPERTY()
	float rotationTicker;

protected:
	virtual void BeginPlay() override;
};

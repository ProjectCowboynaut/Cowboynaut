// you no take candle!

#include "EnemySpawner.h"


// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	dronesSpawned = 0;
	Super::BeginPlay();
	SpawnEnemy();
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!boss)
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), foundActors);
		for (int i = 0; i < foundActors.Num(); i++)
		{
			AEnemy* thisDrone = Cast<AEnemy>(foundActors[i]);
			if (thisDrone->isBoss) boss = thisDrone;
		}
	}
}

void AEnemySpawner::StartSpawning(float delay, float freq)
{
	if (delay == 0)
	{
		SpawnEnemy();
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawner::SpawnEnemy, freq, true);
	}
	else
	{
		FTimerDelegate TimerDel;

		FTimerHandle TimerHandle;

		//Binding the function with specific values
		TimerDel.BindUFunction(this, FName("StartSpawning"), 0, freq);
		//Calling MyUsefulFunction after 5 seconds without looping
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, delay, false);
	}
}

void AEnemySpawner::StopSpawning()
{
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawner::SpawnEnemy, 0, false);
}

void AEnemySpawner::SpawnEnemy()
{
	if (!boss)
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), foundActors);
		for (int i = 0; i < foundActors.Num(); i++)
		{
			AEnemy* thisDrone = Cast<AEnemy>(foundActors[i]);
			if (thisDrone->isBoss)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "illegaler charakter");
				boss = thisDrone;
			}
			//else GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "illegal character");
		}
	}

	FActorSpawnParameters spawnInfo;
	spawnInfo.bNoFail = true;
	FRotator rot = FRotator(.0f, .0f, 0.f);
	FVector spawnLoc = GetActorLocation();
	float rnd1 = FMath::RandRange(-250.f, 250.f);
	float rnd2 = FMath::RandRange(-250.f, 250.f);
	spawnLoc = FVector(spawnLoc.X + rnd1, spawnLoc.Y+rnd2, spawnLoc.Z);

	AEnemy* enemy = GetWorld()->SpawnActor<AEnemy>(EnemyClass, spawnLoc, rot, spawnInfo);
	if (enemy)
	{
		enemy->enemyType = EnemyType::EnemyBossSpawn;
		enemy->SetActorScale3D(FVector(1.3f, 1.3f, 1.3f));
		enemy->health = enemy->healthBase;
		enemy->attackRatio = enemy->attackRatioBase;
		enemy->SpawnDefaultController();				// wont move without.. obviously
		if (boss) 
		{
			boss->bossDrones.Add(enemy);
			boss->bossDronesSpawnedThisPhase++;
		}
		else GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "couldn't add bossdrone!");
		dronesSpawned++;

	}
	
}


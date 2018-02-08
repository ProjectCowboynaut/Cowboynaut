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
	Super::BeginPlay();
	SpawnEnemy();
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("fak-spawn")));
	FActorSpawnParameters spawnInfo;
	AEnemy* enemy = GetWorld()->SpawnActor<AEnemy>(EnemyClass, GetActorLocation(), GetActorRotation(), spawnInfo);
}


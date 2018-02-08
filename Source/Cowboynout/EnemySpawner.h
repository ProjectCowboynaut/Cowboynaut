// you no take candle!

#pragma once

#include "Cowboynout.h"
#include "Enemy.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class COWBOYNOUT_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void StartSpawning(float delay, float freq);

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void StopSpawning();

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void SpawnEnemy();

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	TSubclassOf<class AEnemy> EnemyClass;

	FTimerHandle SpawnTimerHandle;
};

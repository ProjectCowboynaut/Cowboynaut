// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CowboynoutGameMode.generated.h"

UCLASS(minimalapi)
class ACowboynoutGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACowboynoutGameMode();

	/*
	
	void InitiateBossEvent();

	void SpawnBoss();

	void SpawnWave();

	*/

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void StartSpawn(float minDelay, float maxDelay, float freq);

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void StartBossSpawn(float minDelay, float maxDelay, float freq, TArray<AEnemySpawner*> spawnPoints);

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void StopSpawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bossIsSpawning;


};




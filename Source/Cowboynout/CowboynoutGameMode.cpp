// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "CowboynoutGameMode.h"
#include "CowboynoutPlayerController.h"
#include "CowboynoutCharacter.h"
#include "EnemySpawner.h"
#include "UObject/ConstructorHelpers.h"

ACowboynoutGameMode::ACowboynoutGameMode()
{

	// use our custom PlayerController class
	PlayerControllerClass = ACowboynoutPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Characters/BP_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ACowboynoutGameMode::StartSpawn(float minDelay, float maxDelay, float freq)
{
	for (TActorIterator<AEnemySpawner> StartItr(GetWorld()); StartItr; ++StartItr)
	{
		float f = FMath::RandRange(minDelay, maxDelay);
		StartItr->StartSpawning(f, freq);
	}
}

void ACowboynoutGameMode::StartBossSpawn(float minDelay, float maxDelay, float freq, TArray<AEnemySpawner*> spawnPoints)
{
	for (int i = 0; i < spawnPoints.Num(); i++) 
	{
		// while (spawnPoints[i]->dronesSpawned 
		float f = FMath::RandRange(minDelay, maxDelay);
		spawnPoints[i]->StartSpawning(f, freq);
	}
}

void ACowboynoutGameMode::StopSpawn()
{
	for (TActorIterator<AEnemySpawner> StartItr(GetWorld()); StartItr; ++StartItr)
	{
		StartItr->StopSpawning();
	}
}

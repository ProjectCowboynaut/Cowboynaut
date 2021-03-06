// Fill out your copyright notice in the Description page of Project Settings.

#include "Loot.h"


// Sets default values for this component's properties
ULoot::ULoot() {
	PrimaryComponentTick.bCanEverTick = true;

	dropChanceA = .7f;
	dropChanceB = .8f;
	dropChanceC = .7f;
}

// Called when the game starts
void ULoot::BeginPlay() {
	Super::BeginPlay();
	this->Activate();							// auto activation == true;
}

// Called every frame
void ULoot::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULoot::DropChance(FVector spawnLoc, AEnemy* enemyChar) {

	// temp fix

	if (enemyChar) enemyType = enemyChar->type;
	else enemyType = 1;
	
	// adjust loot gain for enemy types
	int lootMass = 1;
	// trash mob
	if (enemyType == 1)
		lootMass = 1;
	// some other mob
	else if (enemyType == 2)
		lootMass = 2;
	// boss mob
	else if (enemyType == 666)
		lootMass = 4;

	// dropchance chip A
	rnd = FMath::RandRange(.0f, 1.0f);
	for (int i = 0; i < lootMass; i++){
		if (rnd < dropChanceA) {
			
			LootDropA(spawnLoc);
		}
	}

	// dropchance chip B
	rnd = FMath::RandRange(.0f, 1.0f);
	for (int i = 0; i < lootMass; i++) {
		if (rnd < dropChanceB) {
			LootDropB(spawnLoc);
		}
	}

	// dropchance chip C
	rnd = FMath::RandRange(.0f, 1.0f);
	for (int i = 0; i < lootMass; i++) {
		if (rnd < dropChanceC) {
			LootDropC(spawnLoc);
		}
	}
}

void ULoot::LootDropA(FVector spawnLoc) {
	FRotator rot;
	FActorSpawnParameters spawnInfo;
	if (droppedItemA) {
		AActor* theDrop = GetWorld()->SpawnActor<AActor>(droppedItemA, spawnLoc, rot, spawnInfo);
	}
	else GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "404: no dropped item found!");
}

void ULoot::LootDropB(FVector spawnLoc) {
	FRotator rot;
	FActorSpawnParameters spawnInfo;
	if (droppedItemB) {
		AActor* theDrop = GetWorld()->SpawnActor<AActor>(droppedItemB, spawnLoc, rot, spawnInfo);
	}
	else GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "404: no dropped item found!");
}


void ULoot::LootDropC(FVector spawnLoc) {
	FRotator rot;
	FActorSpawnParameters spawnInfo;
	if (droppedItemC) {
		AActor* theDrop = GetWorld()->SpawnActor<AActor>(droppedItemC, spawnLoc, rot, spawnInfo);
	}
	else GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "404: no dropped item found!");
}
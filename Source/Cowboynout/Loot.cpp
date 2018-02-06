// Fill out your copyright notice in the Description page of Project Settings.

#include "Loot.h"


ULoot::ULoot() {
	PrimaryComponentTick.bCanEverTick = true;

	// set base loot chances
	dropChanceA = .7f;
	dropChanceB = .8f;
	dropChanceC = .7f;
	dropChanceH = .3f;
	dropChanceG = .3f;
}

void ULoot::BeginPlay() {
	Super::BeginPlay();
	this->Activate();							// auto activation == true;
}

void ULoot::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULoot::DropChance(FVector spawnLoc, AEnemy* enemyChar) {

	// adjust loot gain for enemy types
	int lootMass = 1;

	// trash mob
	if (enemyChar->enemyType == EnemyType::EnemyBase)
		lootMass = 1;
	// some other mob
	else if (enemyChar->enemyType == EnemyType::EnemyElite)
		lootMass = 2;
	// boss mob
	else if (enemyChar->enemyType == EnemyType::EnemyBoss)
		lootMass = 4;

	// dropchance chip A
	rnd = FMath::RandRange(.0f, 1.0f);
	for (int i = 0; i < lootMass; i++){
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "loot a( " + FString::SanitizeFloat(dropChanceA) + "): " + FString::SanitizeFloat(rnd));
		if (rnd < dropChanceA) {
			LootDropA(spawnLoc);
		}
	}

	// dropchance chip B
	rnd = FMath::RandRange(.0f, 1.0f);
	for (int i = 0; i < lootMass; i++) {
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "loot b( " + FString::SanitizeFloat(dropChanceB) + "): " + FString::SanitizeFloat(rnd));
		if (rnd < dropChanceB) {
			LootDropB(spawnLoc);
		}
	}

	// dropchance chip C
	rnd = FMath::RandRange(.0f, 1.0f);
	for (int i = 0; i < lootMass; i++) {
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "loot b( " + FString::SanitizeFloat(dropChanceB) + "): " + FString::SanitizeFloat(rnd));
		if (rnd < dropChanceC) {
			LootDropC(spawnLoc);
		}
	}

	// dropchance health
	rnd = FMath::RandRange(.0f, 1.0f);
	for (int i = 0; i < lootMass; i++) {
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "loot h: " + FString::SanitizeFloat(rnd));
		if (rnd < dropChanceH) {
			LootDropH(spawnLoc);
		}
	}

	// dropchance nades
	rnd = FMath::RandRange(.0f, 1.0f);
	for (int i = 0; i < lootMass; i++) {
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "loot g: " + FString::SanitizeFloat(rnd));
		if (rnd < dropChanceG) {
			LootDropG(spawnLoc);
		}
	}
}

void ULoot::LootDropA(FVector spawnLoc) {
	FRotator rot = FRotator(90, 0, 0);
	FActorSpawnParameters spawnInfo;
	if (droppedItemA) {
		ADrop* theDrop = GetWorld()->SpawnActor<ADrop>(droppedItemA, spawnLoc, rot, spawnInfo);
		if (theDrop) theDrop->lootType = LootType::LootChipA;
	}
	else GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "404: no dropped item found! [A]");
}

void ULoot::LootDropB(FVector spawnLoc) {
	FRotator rot = FRotator(90, 0, 0);
	FActorSpawnParameters spawnInfo;
	if (droppedItemB) {
		ADrop* theDrop = GetWorld()->SpawnActor<ADrop>(droppedItemB, spawnLoc, rot, spawnInfo);
		if (theDrop) theDrop->lootType = LootType::LootChipB;
	}
	else GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "404: no dropped item found! [B]");
}

void ULoot::LootDropC(FVector spawnLoc) {
	FRotator rot = FRotator(90, 0, 0);
	FActorSpawnParameters spawnInfo;
	if (droppedItemC) {
		ADrop* theDrop = GetWorld()->SpawnActor<ADrop>(droppedItemC, spawnLoc, rot, spawnInfo);
		if (theDrop) theDrop->lootType = LootType::LootChipC;
	}
	else GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "404: no dropped item found! [C]");
}

void ULoot::LootDropH(FVector spawnLoc) {
	FRotator rot = FRotator(90, 0, 0);
	FActorSpawnParameters spawnInfo;
	if (droppedItemH) {
		ADrop* theDrop = GetWorld()->SpawnActor<ADrop>(droppedItemH, spawnLoc, rot, spawnInfo);
		if (theDrop) theDrop->lootType = LootType::LootHealth;
	}
	else GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "404: no dropped item found! [H]");
}

void ULoot::LootDropG(FVector spawnLoc) {
	FRotator rot = FRotator(90, 0, 0);
	FActorSpawnParameters spawnInfo;
	if (droppedItemG) {
		ADrop* theDrop = GetWorld()->SpawnActor<ADrop>(droppedItemG, spawnLoc, rot, spawnInfo);
		if (theDrop) theDrop->lootType = LootType::LootNade;
	}
	else GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "404: no dropped item found! [G]");
}
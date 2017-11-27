// Fill out your copyright notice in the Description page of Project Settings.

#include "Loot.h"


// Sets default values for this component's properties
ULoot::ULoot()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void ULoot::BeginPlay()
{
	Super::BeginPlay();
	this->Activate();							// auto activation == true;
	// ...
}

// Called every frame
void ULoot::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULoot::DropChance(FVector spawnLoc) {

	// dropchance minor chip
	rnd = FMath::RandRange(.0f, 1.0f);
	
	if (rnd < dropChanceA) {
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Purple, "m: " + FString::SanitizeFloat(rnd) + " < " + FString::SanitizeFloat(dropChanceMinorChip));
		LootDropA(spawnLoc);
	}
	//else GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White, "m: " + FString::SanitizeFloat(rnd) + " < " + FString::SanitizeFloat(dropChanceMinorChip));

	// dropchance major chip skill one
	rnd = FMath::RandRange(.0f, 1.0f);
	if (rnd < dropChanceB) {
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Purple, "A: " + FString::SanitizeFloat(rnd) + " < " + FString::SanitizeFloat(dropChanceMajorChipA));
		LootDropB(spawnLoc);
	}
	//else GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White, "m: " + FString::SanitizeFloat(rnd) + " < " + FString::SanitizeFloat(dropChanceMinorChip));

	// dropchance major chip skill two
	rnd = FMath::RandRange(.0f, 1.0f);
	if (rnd < dropChanceC) {
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Purple, "B: " + FString::SanitizeFloat(rnd) + " < " + FString::SanitizeFloat(dropChanceMajorChipB));
		LootDropC(spawnLoc);
	}
	//else GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White, "m: " + FString::SanitizeFloat(rnd) + " < " + FString::SanitizeFloat(dropChanceMinorChip));
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
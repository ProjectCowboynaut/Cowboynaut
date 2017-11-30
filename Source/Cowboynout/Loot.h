// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cowboynout.h"
#include "Components/ActorComponent.h"
#include "Enemy.h"
#include "Loot.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COWBOYNOUT_API ULoot : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULoot();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void DropChance(FVector spawnLoc, AEnemy* enemyChar);

	UFUNCTION()
	void LootDropA(FVector spawnLoc);

	UFUNCTION()
	void LootDropB(FVector spawnLoc);

	UFUNCTION()
	void LootDropC(FVector spawnLoc);

	UPROPERTY()
	float dropChanceA = 1.f;

	UPROPERTY()
	float dropChanceB = 1.f;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite)
	float dropChanceC = 1.f;

	UPROPERTY()
	float rnd;
	
	UPROPERTY()
	int enemyType;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AActor> droppedItemA;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AActor> droppedItemB;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AActor> droppedItemC;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};

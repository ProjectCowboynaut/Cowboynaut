// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cowboynout.h"
#include "Components/ActorComponent.h"
#include "Enemy.h"
#include "Drop.h"
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

	UFUNCTION()
	void LootDropH(FVector spawnLoc);

	UFUNCTION()
	void LootDropG(FVector spawnLoc);

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite)
	float dropChanceA;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite)
	float dropChanceB;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite)
	float dropChanceC;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite)
	float dropChanceH;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite)
	float dropChanceG;

	UPROPERTY()
	float rnd;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AActor> droppedItemA;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AActor> droppedItemB;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AActor> droppedItemC;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AActor> droppedItemH;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AActor> droppedItemG;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};

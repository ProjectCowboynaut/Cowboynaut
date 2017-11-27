// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cowboynout.h"
#include "CowboynoutCharacter.h"
#include "AIController.h"
#include "Perception/AISense.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class COWBOYNOUT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:

	AEnemy();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void MouseOverBegin(UPrimitiveComponent* TouchedComponent);

	UFUNCTION(BlueprintCallable)
	void MouseOverEnd();

	UFUNCTION()
	void SetNewMoveDestination(const FVector DestLocation);

	UFUNCTION()
	void RotateCharacter(const FVector DestLocation);

	UFUNCTION()
	void Damage(int dmg);

	UFUNCTION(BlueprintCallable)
	void SenseStuff(TArray<AActor*> testActors);

	UPROPERTY()
	ACowboynoutCharacter* player;

	UPROPERTY()
	int chipsA = 0;

	UPROPERTY() 
	int chipsB = 0;

	UPROPERTY()
	int chipsC = 0;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void Die();

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "EnemyStats")
	int health = 100;

	//behaviour variables
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "EnemyStats")
	int behaviour = 0;							// 0 = go combat range, attack on sight; 1 = alert others; 2 = go close range, explode

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "EnemyStats")
	int type = 0;								// 0 = trash mob; 1 = boss mob

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "EnemyStats")
	int state = 0;								// 0 = passiv; 1 = allerted, 2 = attacking/triggered

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "EnemyStats")
	int armor = 0;								// used to check how many enemies can be punshed through with 1 shot	
};

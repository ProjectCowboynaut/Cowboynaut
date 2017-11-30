// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cowboynout.h"
#include "CowboynoutCharacter.h"
#include "AIController.h"
#include "Perception/AISense.h"
#include "GameFramework/Character.h"
#include "Projectile.h"
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

	UFUNCTION()
	void DoAPeriodicCheck();

	UFUNCTION(BlueprintCallable)
	void Attack();

	UPROPERTY()
	float attackCD;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = "Character")
	USceneComponent* muzzleLocation;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere)
	float attackRatio;

	UPROPERTY(EditAnywhere)
	float loopTime;

	UPROPERTY()
	bool deathTimerActive;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "EnemyStats")
	int type;								// 0 = not initialized type; 1 = trash mob; 666 = boss mob

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "EnemyStats")
	int health;

	//behaviour variables
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "EnemyStats")
	int behaviour;							// 0 = go combat range, attack on sight; 1 = alert others; 2 = go close range, explode

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "EnemyStats")
	int state;								// 0 = passiv; 1 = allerted, 2 = attacking/triggered

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "EnemyStats")
	int armor;								// used to check how many enemies can be punshed through with 1 shot	

	UPROPERTY()
	ACowboynoutCharacter* playerChar;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Loot")
	int chipsA;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Loot")
	int chipsB;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Loot")
	int chipsC;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Combat")
	bool canSeePlayer;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite)
	float timer;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite)
	bool timerActive;
	
	UFUNCTION()
	void Die();

protected:
	
	virtual void BeginPlay() override;
};

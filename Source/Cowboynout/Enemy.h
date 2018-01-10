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
	void DestroyShield();

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite)
	bool bossFightActive;

	UPROPERTY()
	float bossEffectTimer;

	UPROPERTY()
	float bossEffectTimerActive;

	UFUNCTION(BlueprintCallable)
	void Attack();

	UPROPERTY()
	float attackCD;

	// find all Pillars
	UPROPERTY(EditDefaultsOnly, Category = "BossFight")
	TSubclassOf<AActor> PillarBPClass; // Needs to be populated somehow (e.g. by exposing to blueprints as uproperty and setting it there

	UPROPERTY(VisibleAnywhere, Category = "BossFight")
	TArray<AActor*> foundPillars;

	UPROPERTY()
	int pillars;

	UPROPERTY()
	int pillarsToActivate;

	UPROPERTY()
	int pillarsActive;

	UPROPERTY()
	int pillarsLastActive;

	UPROPERTY()
	int pa;

	UPROPERTY()
	int pa2;

	UPROPERTY()
	int pa_;

	UPROPERTY()
	int pa2_;

	UFUNCTION()
	void BossFight(float deltaTime);

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = "Character")
	USceneComponent* muzzleLocation;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY()
	int lastRnd;

	// base stats for enemy types
	UPROPERTY(EditAnywhere, Category = "EnemyType Stats")
	float attackRatioBase;

	UPROPERTY(EditAnywhere, Category = "EnemyType Stats")
	float attackRatioElite;

	UPROPERTY(EditAnywhere, Category = "EnemyType Stats")
	float attackRatioBoss;

	UPROPERTY(EditAnywhere, Category = "EnemyType Stats")
	float damageBase;

	UPROPERTY(EditAnywhere, Category = "EnemyType Stats")
	float damageElite;

	UPROPERTY(EditAnywhere, Category = "EnemyType Stats")
	float damageBoss;

	UPROPERTY(EditAnywhere, Category = "EnemyType Stats")
	float healthBase;

	UPROPERTY(EditAnywhere, Category = "EnemyType Stats")
	float healthElite;

	UPROPERTY(EditAnywhere, Category = "EnemyType Stats")
	float healthBoss;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "EnemyStats")
	float attackRatio;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "EnemyStats")
	int health;

	UPROPERTY(EditAnywhere)
	float loopTime;

	UPROPERTY()
	bool deathTimerActive;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "EnemyStats")
	int type;								// 0 = not initialized type; 1 = trash mob; 666 = boss mob
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "EnemyStats")
	bool shieldOneActive;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "EnemyStats")
	float shieldOne;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "EnemyStats")
	bool shieldTwoActive;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "EnemyStats")
	float shieldTwo;
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "EnemyStats")
	bool shieldThreeActive;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "EnemyStats")
	float shieldThree;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "EnemyStats")
	bool shieldFourActive;
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "EnemyStats")
	float shieldFour;

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

	// sound stuff

	// Soundstuff
	UFUNCTION()
	void PlaySound(int sound);

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* soundSkill1a;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* soundSkill1b;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* soundSkill1c;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* soundHit;

protected:
	
	virtual void BeginPlay() override;
};

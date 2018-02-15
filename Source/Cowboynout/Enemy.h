// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cowboynout.h"
#include "CowboynoutCharacter.h"
#include "AIController.h"
#include "AAIAgent.h"
#include "Perception/AISense.h"
#include "GameFramework/Character.h"
#include "Projectile.h"
#include "Enemy.generated.h"


UENUM(Blueprintable)
enum class EnemyType : uint8
{
	EnemyBase UMETA(DisplayName = "Base Drone"),
	EnemyElite UMETA(DisplayName = "Elite Drone"),
	EnemyBossSpawn UMETA(DisplayName = "Boss Spawn Drone"),
	EnemyBoss UMETA(DisplayName = "Boss Drone")
	
};

UCLASS()
class COWBOYNOUT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:

	AEnemy();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "EnemyType Stats")
	bool isBoss;

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

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite)
	bool bossFightActive;

	UPROPERTY(VisibleAnywhere, Category = "Level Stuff")
	int bossDronesSpawnedThisPhase;

	UPROPERTY(VisibleAnywhere, Category = "Level Stuff")
	int bossDronesToSpawnThisPhase;

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

	UPROPERTY(VisibleAnywhere, Category = "BossFight")
	TArray<AActor*> bossDrones;

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

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly)
	float bossHealth;

	UFUNCTION(BlueprintCallable)
	float GetBossHealth();

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly)
	float bossHealthMax;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> brokenDroneMesh;

	// base stats for enemy types
	UPROPERTY(EditAnywhere, Category = "EnemyType Stats")
	float attackRatioBase;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly)
	bool vincible = false;

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

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "EnemyStats")
	int health;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "EnemyStats")
	int healthMax;

	UPROPERTY(EditAnywhere)
	float loopTime;

	UPROPERTY()
	bool deathTimerActive;

	//UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "EnemyStats")
	//int type;								// 0 = not initialized type; 1 = trash mob; 666 = boss mob

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "EnemyStats")
	EnemyType enemyType;
	
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

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "EnemyType Stats")
	bool isFriendly;
	
	UFUNCTION()
	void Die();

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

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* bossSoundSkill1a;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* bossSoundSkill1b;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* bossSoundSkill1c;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* bossSoundHit;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* bossSoundShieldOff;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* bossSoundShieldOn;

	UFUNCTION(BlueprintCallable)
	void FollowActor(AActor* actor, float deltaTime);

	UPROPERTY(EditDefaultsOnly, Category = "particles")
	UParticleSystem* dmgEffectParticle;

	UFUNCTION(BlueprintNativeEvent, Category = "AI|Enemy")
	float GetFollowConstraint();
	virtual float GetFollowConstraint_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "AI|Enemy")
	float GetCoverConstraint();
	virtual float GetCoverConstraint_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "AI|Enemy")
	float GetRoamConstraint();
	virtual float GetRoamConstraint_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "AI|Enemy")
	float GetHealConstraint();
	virtual float GetHealConstraint_Implementation();

protected:
	
	virtual void BeginPlay() override;
};

// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Cowboynout.h"
#include "GameFramework/Character.h"
#include "Projectile.h"
#include "Grenade.h"
#include "CowboynoutCharacter.generated.h"

UCLASS(Blueprintable)
class ACowboynoutCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACowboynoutCharacter();

	/** Returns CollisionComp subobject **/
	FORCEINLINE USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = "Character")
	USphereComponent* CollisionComp;

	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE 
	class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }

	/** Returns CameraBoom subobject **/
	FORCEINLINE 
	class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns CursorToWorld subobject **/
	FORCEINLINE 
	class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	UFUNCTION()
	void Damage(int dmg);

	// SkillBlueprints (Tier 1-3 each)
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AProjectile> ProjectileClassT1;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AProjectile> ProjectileClassT2;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AProjectile> ProjectileClassT3;

	UPROPERTY()
	AGrenade* nade;

	UPROPERTY()
	FVector nadeLoc;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AGrenade> GrenadeClassT1;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AGrenade> GrenadeClassT2;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AGrenade> GrenadeClassT3;

	UPROPERTY()
	bool explodeNade;

	// Skills Levels
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	int skillLvlOne;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	int skillLvlTwo;

	UFUNCTION()
	void FireSkillOne();

	UFUNCTION()
	void FireSkillTwo(int teleport);

	UFUNCTION()
	void SetTarget(int targetStatus);

	UFUNCTION(BlueprintCallable)
	int GetChipsA();

	UFUNCTION(BlueprintCallable)
	int GetChipsB();

	UFUNCTION(BlueprintCallable)
	int GetChipsC();

	UFUNCTION(BlueprintCallable)
	float GetHealth();

	UFUNCTION(BlueprintCallable)
	void ConvertChipSkillA();

	UFUNCTION(BlueprintCallable)
	void ConvertChipSkillB();

	UFUNCTION(BlueprintCallable)
	void ConvertChipStatA();

	UFUNCTION(BlueprintCallable)
	void ConvertChipStatB();

	UFUNCTION(BlueprintCallable)
	void ConvertChipStatC();

	UFUNCTION()
	void Die();

	// animations triggers
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Animations")
	bool animDead = false;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category="Animations")
	bool animRunning = false;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Animations")
	bool animShooting = false;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Animations")
	bool animIdle = false;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Animations")
	bool animDying = false;
	
	//  projectiles spawn
	UPROPERTY(VisibleDefaultsOnly, Category = "Character")
	USceneComponent* muzzleLocation;

	UPROPERTY(VisibleDefaultsOnly, Category = "Character")
	USceneComponent* muzzleLocationL;

	UPROPERTY(VisibleDefaultsOnly, Category = "Character")
	USceneComponent* muzzleLocationR;

	UPROPERTY()
	float deathTimer;

	UPROPERTY()
	float deathTimerFull;

	UPROPERTY()
	bool dead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	int hasTarget;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")			// 0: no target; 1: enemy target; 2: usable
	FString targetString;

	// "Inventory"
	UPROPERTY(EditAnywhere, Category= "Loot")
	int chipsA;

	UPROPERTY(EditAnywhere, Category = "Loot")
	int chipsB;

	UPROPERTY(EditAnywhere, Category = "Loot")
	int chipsC;

	// player stats
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "PlayerStats")
	float life;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "PlayerStats")
	float lifeMax;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	int speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	int attack;

	// player stat gains
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "PlayerStats")
	float lifeGainPerLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	int speedGainPerLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	int attackGainPerLevel;

	// Soundstuff
	UFUNCTION()
	void PlaySound(int sound);

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* soundSkill1;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* soundSkill2shot;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* soundSkill2explosion;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* soundLowLife;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* soundDead;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	float lifeWarningValue;

private:

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

//protected:

};


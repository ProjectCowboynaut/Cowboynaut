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

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AProjectile> ProjectileClass;

	UFUNCTION()
	void FireSkillOne();

	UFUNCTION()
	void FireSkillTwo();

	UFUNCTION()
	void SetTarget(bool targetStatus);

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
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category="Animations")
	bool animRunning;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Animations")
	bool animShooting;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Animations")
	bool animIdle;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Animations")
	bool animDying;
	
	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = "Character")
	USceneComponent* muzzleLocation;

	/** nade class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AGrenade> GrenadeClass;

	UPROPERTY(BlueprintReadOnly, Category = "PlayerStats")
	bool hasTarget = false;

	UPROPERTY(EditAnywhere, Category= "Loot")
	int chipsA = 0;

	UPROPERTY(EditAnywhere, Category = "Loot")
	int chipsB = 0;

	UPROPERTY(EditAnywhere, Category = "Loot")
	int chipsC = 0;

	// stats
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "PlayerStats")
	float life = 100;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "PlayerStats")
	float lifeMax = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	int speed = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	int attack = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	int skillLvlTwo = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	int skillLvlOne = 1;


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


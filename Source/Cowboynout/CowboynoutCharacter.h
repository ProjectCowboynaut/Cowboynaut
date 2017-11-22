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

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = "Character")
	class USceneComponent* muzzleLocation;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AGrenade> GrenadeClass;


	UPROPERTY(EditAnywhere, Category = "PlayerStats")
	bool hasTarget = false;

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

protected:

	// stats
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "PlayerStats")
	int life = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	int speed = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	int attack = 1;


};


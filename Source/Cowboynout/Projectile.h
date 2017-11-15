// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cowboynout.h"
#include "GameFramework/Actor.h"
#include "CowboynoutCharacter.h"
#include "Projectile.generated.h"


UCLASS()
class AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();
	
	UFUNCTION()
	void DebugMsg(FString msg, float dTime, FColor clr);


	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* lazor;

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	/** Projectile Damage */
	UPROPERTY(EditAnywhere, Category = "Projectile")
	int projectileDamage = 10.f;

	UFUNCTION()
	void Initialize(int damage);

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
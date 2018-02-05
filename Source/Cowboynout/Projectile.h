// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cowboynout.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UENUM(Blueprintable)
enum class BulletType : uint8
{
	PlayerBullet UMETA(DisplayName = "Player Bullet"),
	EnemyBullet UMETA(DisplayName = "Enemy Bullet")
};

UCLASS()
class AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();

	UFUNCTION()
	void DebugMsg(FString msg, float dTime, FColor clr);

	UFUNCTION()
	void Initialize(int damage);

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Returns CollisionComp subobject **/
	FORCEINLINE USphereComponent* GetCollisionComp() const {
		return CollisionComp;
	}

	UPROPERTY()
	bool debugEnabled;

	// Begin PlayerController interface
	UFUNCTION()
	virtual void Tick(float deltaTime) override;

	/*UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Projectile")
	FString bulletType;*/

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category="Projectile")
	bool playerProjectile;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Projectile")
	bool enemyProjectile;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> hitEnemies;

	/** Returns ProjectileMovement subobject **/
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const {
		return ProjectileMovement;
	}

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* lazor;

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	/** Projectile Damage */
	UPROPERTY(EditAnywhere, Category = "Projectile")
	int projectileDamage;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	int penetration;

	UFUNCTION()
	void setBulletStats();

	UPROPERTY()
	bool statsSet;

	UPROPERTY()
	BulletType bulletType;

};
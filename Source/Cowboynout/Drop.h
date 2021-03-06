// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cowboynout.h"
#include "GameFramework/Actor.h"
#include "Drop.generated.h"

UCLASS()
class COWBOYNOUT_API ADrop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrop();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	int type;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = "Loot")
	USphereComponent* CollisionComp;
};

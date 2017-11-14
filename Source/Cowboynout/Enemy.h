// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class COWBOYNOUT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetNewMoveDestination(const FVector DestLocation);
	void RotateCharacter(const FVector DestLocation);
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Character") AActor* myActor;
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Character") APawn* MyPawn;
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Character") UNavigationSystem* NavSys;
};
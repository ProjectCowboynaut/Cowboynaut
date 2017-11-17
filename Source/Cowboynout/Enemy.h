// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cowboynout.h"
#include "CowboynoutCharacter.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class COWBOYNOUT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:

	AEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Pawn|Character")
	void MouseOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void SetNewMoveDestination(const FVector DestLocation);
	UFUNCTION()
	void RotateCharacter(const FVector DestLocation);

protected:

	virtual void BeginPlay() override;

};

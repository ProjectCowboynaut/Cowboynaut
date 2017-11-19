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

	UFUNCTION(BlueprintCallable)
	void MouseOverBegin(UPrimitiveComponent* TouchedComponent);

	UFUNCTION(BlueprintCallable)
	void MouseOverEnd();

	UFUNCTION()
	void SetNewMoveDestination(const FVector DestLocation);

	UFUNCTION()
	void RotateCharacter(const FVector DestLocation);

	UPROPERTY()
	ACowboynoutCharacter* player;

protected:

	virtual void BeginPlay() override;

	//UPROPERTY()
	//ACharacter* player;
};

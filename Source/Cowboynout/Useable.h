// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cowboynout.h"
#include "CowboynoutCharacter.h"
#include "GameFramework/Actor.h"
#include "Useable.generated.h"

UCLASS()
class COWBOYNOUT_API AUseable : public AActor
{
	GENERATED_BODY()
	
public:	
	AUseable();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	ACowboynoutCharacter* playerChar;

	UFUNCTION(BlueprintCallable)
	void MouseOverBegin(UPrimitiveComponent* TouchedComponent);

	UFUNCTION(BlueprintCallable)
	void MouseOverEnd();

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly)
	bool isMouseOver;

	UFUNCTION(BlueprintCallable)
	void AllowUpgrade(int state);

protected:

	virtual void BeginPlay() override;
};

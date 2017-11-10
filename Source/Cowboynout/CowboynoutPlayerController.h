// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CowboynoutPlayerController.generated.h"

UCLASS()
class ACowboynoutPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACowboynoutPlayerController();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface


	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	
	// Input for stationairy attacks 
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category="Movement|Keys") bool isStationairy;

	void OnSetStationairyPressed();
	void OnSetStationairyReleased();

	// Rotation & Movement
	void MoveTo();
	void RotatePlayer();

	// Attack Variables
	void MainFire();
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat") bool saveAttack;
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat") bool isAttacking;
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat") bool combat;
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat") bool hasTarget;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat") float shotsPerSecond;
	
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat") bool skillActive = false;

	//skill one
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat") float timerSkillOne = 0; 
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat") float activeTimerSkillOne = 0;
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat") bool skillOneCD = false;
	//skill two
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat") float timerSkillTwo = 0;
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat") float activeTimerSkillTwo = 0;
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat") bool skillTwoCD = false;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat") FVector gunPosition;

};



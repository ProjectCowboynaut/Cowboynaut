// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Cowboynout.h"
#include "GameFramework/PlayerController.h"
#include "Projectile.h"
#include "CowboynoutCharacter.h"
#include "CowboynoutPlayerController.generated.h"


UCLASS()
class ACowboynoutPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACowboynoutPlayerController();

	// character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	FVector gunPosition;

	// debug 
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Debug")
	float displayTime = .5f;

	UFUNCTION()
	void DebugMsg(FString msg, float dTime, FColor clr);

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;
	
	UPROPERTY()
	ACowboynoutCharacter* cowboy;


	// Begin PlayerController interface
	UFUNCTION()
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION()
	virtual void SetupInputComponent() override;
	// End PlayerController interface


	UFUNCTION()
	void MoveToMouseCursor();
	
	/** Navigate player to the given world location. */
	UFUNCTION()
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	UFUNCTION()
	void OnSetDestinationPressed();

	UFUNCTION()
	void OnSetDestinationReleased();

	UFUNCTION()
	void OnMovementModePressed();

	UFUNCTION()
	void OnMovementModeReleased();

	UFUNCTION()
	void OnSetStationairyPressed();

	UFUNCTION()
	void OnSetStationairyReleased();

	// Rotation
	UFUNCTION()
	void RotatePlayer();

	/** Input handlers for Skills */
	UFUNCTION()
	void OnSkillOnePressed();

	UFUNCTION()
	void OnSkillOneReleased();

	UFUNCTION()
	void OnSkillTwoPressed();

	UFUNCTION()
	void OnSkillTwoReleased();

	UFUNCTION()
	void OnSkillThreePressed();

	UFUNCTION()
	void OnSkillThreeReleased();

	UFUNCTION()
	void SkillOne();

	UFUNCTION()
	void SkillTwo();

	UFUNCTION()
	void SkillTwoTP();

	UFUNCTION()
	void SkillThree();



	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Character")
	AProjectile* projectile;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Character") 
	AActor* myActor;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Character") 
	APawn* MyPawn;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Character") 
	UNavigationSystem* NavSys;


	// combat vars
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Info") 
	bool saveAttack;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Info") 
	bool isAttacking;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Info") 
	bool combat;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Info") 
	bool hasTarget;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Info") 
	float shotsPerSecond;


	// skill one
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	float timerSkillOne = .2f; 

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	float breakSkillOne = .2f;				//  no movement for x seconds on skill

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	float activeTimerSkillOne = .0f;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	bool skillOneCD = false;


	// skill two
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	float timerSkillTwo = 1.25f;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	float breakSkillTwo = .6f;				//  no movement for x seconds on skill

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	float activeTimerSkillTwo = .0f;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	bool skillTwoCD = false;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	bool skillTwoTPCD = false;


	// skill three
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	float timerSkillThree = 2.5f;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	float breakSkillThree = 1.25f;			//  no movement for x seconds on skill

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	float activeTimerSkillThree = .0f;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	bool skillThreeCD = false;


	// movement
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Movement") 
	bool isStationairy = false;				// used when stationairy key is pressed

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Movement")
	bool moveOnly = false;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Movement") 
	bool canMove = true;					// used to break movement on skill use

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Movement") 
	bool isMoving = false;					// durr, used to see if moving
};



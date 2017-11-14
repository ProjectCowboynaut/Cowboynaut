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

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;
	
	UPROPERTY()
	ACowboynoutCharacter* cowboy;

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

	void DebugMsg(FString msg, float displayTime, FColor clr);

	void OnSetStationairyPressed();
	void OnSetStationairyReleased();

	void Stop();

	// Rotation
	void RotatePlayer();

	/** Input handlers for Skills */
	void OnSkillOnePressed();
	void OnSkillOneReleased();
	void OnSkillTwoPressed();
	void OnSkillTwoReleased();
	void OnSkillThreePressed();
	void OnSkillThreeReleased();

	void SkillOne();
	void SkillTwo();
	void SkillTwoTP();
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
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat|Attack") 
	bool saveAttack;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat|Attack") 
	bool isAttacking;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat|Attack") 
	bool combat;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat|Attack") 
	bool hasTarget;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat|Attack") 
	float shotsPerSecond;


	// skill one
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat|Skills") 
	float timerSkillOne = .2f; 

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat|Skills") 
	float breakSkillOne = .2f;			//  no movement for x seconds on skill

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat|Skills") 
	float activeTimerSkillOne = .0f;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat|Skills") 
	bool skillOneCD = false;


	// skill two
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat|Skills") 
	float timerSkillTwo = 1.25f;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat|Skills") 
	float breakSkillTwo = .6f;			//  no movement for x seconds on skill

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat|Skills") 
	float activeTimerSkillTwo = .0f;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat|Skills") 
	bool skillTwoCD = false;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat|Skills") 
	bool skillTwoTPCD = false;


	// skill three
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat|Skills") 
	float timerSkillThree = 2.5f;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat|Skills") 
	float breakSkillThree = 1.25f;			//  no movement for x seconds on skill

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat|Skills") 
	float activeTimerSkillThree = .0f;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Combat|Skills") 
	bool skillThreeCD = false;


	// movement
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Movement|Keys") 
	bool isStationairy;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Movement") 
	bool canMove = true;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Movement") 
	bool isMoving = false;


};



// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Cowboynout.h"
#include "GameFramework/PlayerController.h"
#include "Projectile.h"
#include "CowboynoutCharacter.h"
#include "TextboxUserWidget.h"
#include "CowboynoutPlayerController.generated.h"


UCLASS()
class ACowboynoutPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACowboynoutPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float characterMovementSpeed;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Movement")
	int controleMethod;

	UFUNCTION()
	void DebugMsg(FString msg, float dTime, FColor clr);

	// character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	FVector gunPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> wInfoW;

	UPROPERTY()
	UUserWidget* myInfoW;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> wTextbox;

	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	UTextboxUserWidget* myTextbox;
	//UUserWidget* 
	

	UPROPERTY()
	bool info;

	// debug 
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Debug")
	float displayTime;

	// used when stationairy key is pressed
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Movement")
	bool isStationairy;							

	// not an actual death timer, more the end game timer... sorry
	UPROPERTY()
	float deathTimer;

	UPROPERTY()
	bool endGame;

	UPROPERTY()
	bool canTP;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Level")
	float endCD;

	UPROPERTY()
	float deathTimerFull;

	UPROPERTY()
	bool deathTimerNotSet;

	UPROPERTY()
	float countDown;

	UPROPERTY()
	ACowboynoutCharacter* cowboy;

	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	UFUNCTION()
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void SetupInputComponent() override;
	// End PlayerController interface


	UFUNCTION()
	void OnSetStationairyPressed();

	UFUNCTION()
	void OnSetStationairyReleased();

	UFUNCTION()
	void OnSkillOneLevelUp();
	
	UFUNCTION()
	void OnSkillTwoLevelUp();

	// Rotation
	UFUNCTION()
	void RotatePlayer();

	UFUNCTION()
	void OnLeftMousePressed();

	UFUNCTION()
	void OnLeftMouseReleased();

	UFUNCTION()
	void OnRightMousePressed();

	UFUNCTION()
	void OnRightMouseReleased();

	UFUNCTION()
	void OnSpacePressed();

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

	UFUNCTION()
	void MedPack();

	UFUNCTION()
	void OnSimulateDamagePressed();

	UFUNCTION()
	void OnSimulateDamageReleased();
	
	UFUNCTION()
	void SimulateDamage();

	UFUNCTION()
	void PlusStatA();

	UFUNCTION()
	void PlusStatB();

	UFUNCTION()
	void PlusStatC();

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Character")
	UStaticMeshComponent* meshComp;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Character")
	AProjectile* projectile;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Character") 
	AActor* myActor;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Character") 
	APawn* myLittlePawny;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Character") 
	UNavigationSystem* NavSys;

	// combat vars
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Info") 
	bool saveAttack;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Info") 
	bool isAttacking;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Info")
	bool canUpgrade;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Info") 
	bool combat;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Info") 
	float shotsPerSecond;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills")
	bool isDashing;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills")
	float dashDistanceActual;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills")
	FVector dashStartPoint;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Skills")
	float dashDistanceMax;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills")
	float dashSpeed;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills")
	FVector dashDirection;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills")
	FVector dashTargetLocation;

	// skill one
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	float timerSkillOne; 

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	float breakSkillOne;				//  no movement for x seconds on skill

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	float activeTimerSkillOne;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	bool skillOneCD;

	// skill two
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	float timerSkillTwo;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	float breakSkillTwo;				//  no movement for x seconds on skill

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	float activeTimerSkillTwo;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	bool skillTwoCD;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	bool skillTwoTPCD;

	// skill three
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	float timerSkillThree;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	float breakSkillThree;			//  no movement for x seconds on skill

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	float activeTimerSkillThree;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Skills") 
	bool skillThreeCD;

	// movement

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Movement")
	bool moveOnly;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Movement") 
	bool canMove;					// used to break movement on skill use

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Movement") 
	bool isMoving;					// durr, used to see if moving


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void MoveForward(float axisValue);

	UFUNCTION()
	void MoveRight(float axisValue);

	UFUNCTION()
	void WASDMove(float DeltaTime);

	UPROPERTY()
	FVector2D MovementInput;

	UFUNCTION()
	bool CheckMap();

};



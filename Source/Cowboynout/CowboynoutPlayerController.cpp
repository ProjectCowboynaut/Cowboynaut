// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "CowboynoutPlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "CowboynoutCharacter.h"

ACowboynoutPlayerController::ACowboynoutPlayerController() {
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	gunPosition = FVector(0.f, 15.f, 0.f);
	MyPawn = GetPawn();
}

void ACowboynoutPlayerController::SetupInputComponent() {
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ACowboynoutPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ACowboynoutPlayerController::OnSetDestinationReleased);

	InputComponent->BindAction("MovementMode", IE_Pressed, this, &ACowboynoutPlayerController::OnMovementModePressed);
	InputComponent->BindAction("MovementMode", IE_Released, this, &ACowboynoutPlayerController::OnMovementModeReleased);

	// stationairy mode
	InputComponent->BindAction("StationairyMode", IE_Pressed, this, &ACowboynoutPlayerController::OnSetStationairyPressed);
	InputComponent->BindAction("StationairyMode", IE_Released, this, &ACowboynoutPlayerController::OnSetStationairyReleased);

	// Skill One
	InputComponent->BindAction("SkillOne", IE_Pressed, this, &ACowboynoutPlayerController::OnSkillOnePressed);
	InputComponent->BindAction("SkillOne", IE_Released, this, &ACowboynoutPlayerController::OnSkillOneReleased);
	// Skill Two
	InputComponent->BindAction("SkillTwo", IE_Pressed, this, &ACowboynoutPlayerController::OnSkillTwoPressed);
	InputComponent->BindAction("SkillTwo", IE_Released, this, &ACowboynoutPlayerController::OnSkillTwoReleased);
	// Skill Three
	InputComponent->BindAction("SkillThree", IE_Pressed, this, &ACowboynoutPlayerController::OnSkillThreePressed);
	InputComponent->BindAction("SkillThree", IE_Released, this, &ACowboynoutPlayerController::OnSkillThreeReleased);
}

void ACowboynoutPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);
	
	if (MyPawn) {
		FVector velo = MyPawn->GetVelocity();
		if (velo.X != 0 || velo.Y != 0)
			isMoving = true;
		else
			isMoving = false;
	}

	// keep updating the destination every tick while presed
	if (bMoveToMouseCursor)	{
		MoveToMouseCursor();
	}
	if (!isMoving) {
		RotatePlayer();
	}

	// reset skill CDs if time passed
	if (skillOneCD) {
		activeTimerSkillOne += DeltaTime;
		if (activeTimerSkillOne >= breakSkillOne) {		// enable movement again after break time
			canMove = true;
		}
		if (activeTimerSkillOne >= timerSkillOne) {
			skillOneCD = false;
			activeTimerSkillOne = 0;
		}
	}
	if (skillTwoCD) {
		activeTimerSkillTwo += DeltaTime;
		if (activeTimerSkillTwo >= breakSkillTwo) {		// enable movement again after break time
			canMove = true;	
		}
		if (activeTimerSkillTwo >= timerSkillTwo) {
			skillTwoCD = false;
			skillTwoTPCD = false;
			activeTimerSkillTwo = 0;
		}
	}
	if (skillThreeCD) {
		activeTimerSkillThree += DeltaTime;
		if (activeTimerSkillThree >= breakSkillThree) {	// enable movement again after break time
			canMove = true;	
		}
		if (activeTimerSkillThree >= timerSkillThree) {
			skillThreeCD = false;
			activeTimerSkillThree = 0;
		}
	}
}

void ACowboynoutPlayerController::MoveToMouseCursor() {
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	if (Hit.bBlockingHit && !isStationairy) {
		SetNewMoveDestination(Hit.ImpactPoint);
	}
}

void ACowboynoutPlayerController::SetNewMoveDestination(const FVector DestLocation) {
	if (canMove && !isStationairy) {
		MyPawn = GetPawn();						// needed again for some reason, won't move without.
		if (MyPawn) {
			NavSys = GetWorld()->GetNavigationSystem();
			float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

			if (NavSys && (Distance > 120.0f)) {
				NavSys->SimpleMoveToLocation(this, DestLocation);
			}
		}
	}
}

void ACowboynoutPlayerController::OnSetDestinationPressed() {
	if (canMove && !isStationairy)
		bMoveToMouseCursor = true;
}

void ACowboynoutPlayerController::OnSetDestinationReleased() {
	bMoveToMouseCursor = false;
}

void ACowboynoutPlayerController::OnMovementModePressed() {
	moveOnly = true;
}

void ACowboynoutPlayerController::OnMovementModeReleased() {
	moveOnly = false;
}

void ACowboynoutPlayerController::OnSetStationairyPressed() {
	isStationairy = true;
	AController::StopMovement();
}

void ACowboynoutPlayerController::OnSetStationairyReleased() {
	isStationairy = false;
}

void ACowboynoutPlayerController::OnSkillOnePressed() {
	if (!moveOnly){
		if (Cast<ACowboynoutCharacter>(GetCharacter())->hasTarget || isStationairy) {
			AController::StopMovement();
			RotatePlayer();
			SkillOne();
		}
	}
	else {
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);
		if (Hit.bBlockingHit && !isStationairy) {
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void ACowboynoutPlayerController::OnSkillOneReleased() {

}

/// WiP :: not good, fix later
void ACowboynoutPlayerController::OnSkillTwoPressed() {
	// if skill is active / flying  
	if (skillTwoCD) {
		// port to nade
		//SkillTwoTP();
	}
	else {
		// use skill two
		if (!moveOnly) {
			SkillTwo();
		}
	}
}

void ACowboynoutPlayerController::OnSkillTwoReleased() {

}

void ACowboynoutPlayerController::OnSkillThreePressed() {
	if (!moveOnly) SkillThree();
}

void ACowboynoutPlayerController::OnSkillThreeReleased() {

}

void ACowboynoutPlayerController::RotatePlayer() {
	// Get mouse position on screen
	float xMouse;
	float yMouse;
	GetMousePosition(xMouse, yMouse);

	// Get Character position on screen
	FVector charLoc = GetCharacter()->GetActorLocation();
	FVector2D charInScreen;
	ProjectWorldLocationToScreen(charLoc, charInScreen);

	// Get mouse position relative to the Character.
	FVector2D result;
	result.X = -(yMouse - charInScreen.Y);
	result.Y = xMouse - charInScreen.X;

	// Get angle rotation and rotation Character
	float angle = FMath::RadiansToDegrees(FMath::Acos(result.X / result.Size()));

	if (result.Y < 0)
		angle = 360 - angle;

	FRotator rot(0, angle, 0);

	GetCharacter()->SetActorRotation(rot);
}

void ACowboynoutPlayerController::SkillOne() {
	if (!skillOneCD) {
		cowboy = Cast<ACowboynoutCharacter>(GetCharacter());
		if (cowboy) cowboy->FireSkillOne();
		else DebugMsg("cowboy nullptr", displayTime, FColor::Red);
	}
	else {
		DebugMsg("skill one on CD", displayTime, FColor::Red);
		return;
	}
	AController::StopMovement();						// stop movement
	skillOneCD = true;		// set CD
	
	// play sound at player location
}

void ACowboynoutPlayerController::SkillTwo() {
	if (!skillOneCD) {
		
		cowboy = Cast<ACowboynoutCharacter>(GetCharacter());
		if (cowboy) cowboy->FireSkillTwo();
		else DebugMsg("cowboy nullptr", displayTime, FColor::Red);
	}
	else {
		DebugMsg("skill two on CD", displayTime, FColor::Red);
		return;
	}

	// stop movement
	AController::StopMovement();
	skillTwoCD = true;	// set CD

	// play sound at player location
}

void ACowboynoutPlayerController::SkillTwoTP() {
	// check if nade is still flying
	// get nade pos
	// tele boom player at nade pos

	if (!skillTwoTPCD) {
		DebugMsg("skill two TP activated", displayTime, FColor::Yellow);
		skillTwoTPCD = true;
	} 
	else {
		DebugMsg("skill two TP on CD", displayTime, FColor::Red);
	}
}

void ACowboynoutPlayerController::SkillThree() {
	if (!skillThreeCD) DebugMsg("skill three fired", displayTime, FColor::Yellow);
	else {
		DebugMsg("skill three on CD", displayTime, FColor::Red);
		return;
	}

	// stop movement
	AController::StopMovement();
	skillThreeCD = true;	// set CD
}


// bugs bunny
void ACowboynoutPlayerController::DebugMsg(FString msg, float dTime, FColor clr) {
	GEngine->AddOnScreenDebugMessage(-1, dTime, clr, msg);
}
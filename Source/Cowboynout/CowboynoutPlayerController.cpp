// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "CowboynoutPlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "CowboynoutCharacter.h"


ACowboynoutPlayerController::ACowboynoutPlayerController() {
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	gunPosition = FVector(0.f, 15.f, 0.f);
}


void ACowboynoutPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)	{
		MoveToMouseCursor();
	}
	else {
		RotatePlayer();
	}

	// reset skill CDs if time passed

	if (skillOneCD) {
		activeTimerSkillOne += DeltaTime;
		if (activeTimerSkillOne >= timerSkillOne) {
			skillOneCD = false;
			activeTimerSkillOne = 0;
		}
	}
	else if (skillTwoCD) {
		activeTimerSkillTwo += DeltaTime;
		if (activeTimerSkillTwo >= timerSkillTwo) {
			skillTwoCD = false;
			activeTimerSkillTwo = 0;
		}
	}

}


void ACowboynoutPlayerController::SetupInputComponent() {
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ACowboynoutPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ACowboynoutPlayerController::OnSetDestinationReleased);

	// stationairy mode
	InputComponent->BindAction("StationairyMode", IE_Pressed, this, &ACowboynoutPlayerController::OnSetStationairyPressed);
	InputComponent->BindAction("StationairyMode", IE_Released, this, &ACowboynoutPlayerController::OnSetStationairyReleased);
}


void ACowboynoutPlayerController::MoveToMouseCursor() {
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled()) {
		if (ACowboynoutCharacter* MyPawn = Cast<ACowboynoutCharacter>(GetPawn())) {
			if (MyPawn->GetCursorToWorld())	{
				UNavigationSystem::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else {
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit) {
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}


void ACowboynoutPlayerController::SetNewMoveDestination(const FVector DestLocation) {
	if (!isStationairy) {
		APawn* const MyPawn = GetPawn();
		if (MyPawn)	{
			UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
			float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

			// We need to issue move command only if far enough in order for walk animation to play correctly
			if (NavSys && (Distance > 120.0f)) {
				NavSys->SimpleMoveToLocation(this, DestLocation);
			}
		}
	}
}


void ACowboynoutPlayerController::OnSetDestinationPressed() {
	if (isStationairy) return;

	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}


void ACowboynoutPlayerController::OnSetDestinationReleased() {
	if (isStationairy) return;
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}


void ACowboynoutPlayerController::OnSetStationairyPressed() {
	isStationairy = true;
}


void ACowboynoutPlayerController::OnSetStationairyReleased() {
	isStationairy = false;
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


void ACowboynoutPlayerController::MainFire() {
	//	stop movement
	//	set firetime
	skillOneCD = true;
	//	spawn projectile @gunPosition.worldtransform
	//GetWorld()->SpawnActor<>
	//	play sound at player location
	//	continue movement

}

void ACowboynoutPlayerController::MoveToMouseCursor() {

}
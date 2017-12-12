// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "CowboynoutPlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Perception/AISense.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "CowboynoutCharacter.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Slate.h"

ACowboynoutPlayerController::ACowboynoutPlayerController() {
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	gunPosition = FVector(0.f, 15.f, 0.f);
	MyPawn = GetPawn();

	// ## var init
	info = false;
	displayTime = .5f;
	isStationairy = false;
	deathTimerNotSet = false;
	deathTimer = 10.f;

	// break == "no movement time"; timer == cd on skill
	timerSkillOne = .6f;
	breakSkillOne = .2f;
	activeTimerSkillOne = .0f;
	skillOneCD = false;
	timerSkillTwo = 2.25f;
	breakSkillTwo = 1.f;
	activeTimerSkillTwo = .0f;
	skillTwoCD = false;
	skillTwoTPCD = false;
	timerSkillThree = 2.5f;
	breakSkillThree = 1.25f;
	activeTimerSkillThree = .0f;
	skillThreeCD = false;
	
	moveOnly = false;
	canMove = true;
	isMoving = false;
}

void ACowboynoutPlayerController::SetupInputComponent() {
	// set up gameplay key bindings
	Super::SetupInputComponent();

	//InputComponent->BindAction("SetDestination", IE_Pressed, this, &ACowboynoutPlayerController::OnSetDestinationPressed);
	//InputComponent->BindAction("SetDestination", IE_Released, this, &ACowboynoutPlayerController::OnSetDestinationReleased);

	// movement modes
	InputComponent->BindAction("MovementMode", IE_Pressed, this, &ACowboynoutPlayerController::OnMovementModePressed);
	InputComponent->BindAction("MovementMode", IE_Released, this, &ACowboynoutPlayerController::OnMovementModeReleased);
	InputComponent->BindAction("StationairyMode", IE_Pressed, this, &ACowboynoutPlayerController::OnSetStationairyPressed);
	InputComponent->BindAction("StationairyMode", IE_Released, this, &ACowboynoutPlayerController::OnSetStationairyReleased);

	// skills
	InputComponent->BindAction("SkillOne", IE_Pressed, this, &ACowboynoutPlayerController::OnSkillOnePressed);
	InputComponent->BindAction("SkillOne", IE_Released, this, &ACowboynoutPlayerController::OnSkillOneReleased);
	InputComponent->BindAction("SkillTwo", IE_Pressed, this, &ACowboynoutPlayerController::OnSkillTwoPressed);
	InputComponent->BindAction("SkillTwo", IE_Released, this, &ACowboynoutPlayerController::OnSkillTwoReleased);
	InputComponent->BindAction("SkillThree", IE_Pressed, this, &ACowboynoutPlayerController::OnSkillThreePressed);
	InputComponent->BindAction("SkillThree", IE_Released, this, &ACowboynoutPlayerController::OnSkillThreeReleased);

	// mouse
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &ACowboynoutPlayerController::OnLeftMousePressed);
	InputComponent->BindAction("LeftClick", IE_Released, this, &ACowboynoutPlayerController::OnLeftMouseReleased);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &ACowboynoutPlayerController::OnRightMousePressed);
	InputComponent->BindAction("RightClick", IE_Released, this, &ACowboynoutPlayerController::OnRightMouseReleased);

	// simulate damage
	InputComponent->BindAction("SimulateDamage", IE_Pressed, this, &ACowboynoutPlayerController::OnSimulateDamagePressed);
	InputComponent->BindAction("SimulateDamage", IE_Released, this, &ACowboynoutPlayerController::OnSimulateDamageReleased);

	//UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, sightConfig->GetSenseImplementation(), GetControlledPawn());
}

void ACowboynoutPlayerController::PlayerTick(float deltaTime) {
	Super::PlayerTick(deltaTime);
	
	// end cd timer 

	if (endGame) {
		deathTimer += deltaTime;
		if (endCD == 0 && !deathTimerNotSet) {
			deathTimerFull = deathTimer;
			deathTimerNotSet = true;
		}
		countDown = deathTimerFull - deathTimer;
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::SanitizeFloat(countDown));
		
		if (deathTimer >= deathTimerFull) {
			UGameplayStatics::OpenLevel(this, TEXT("/Game/Maps/WinScreen"), false);
			deathTimer = 0;
		}
	}

	if (MyPawn) {
		FVector velo = MyPawn->GetVelocity();
		if (velo.X != 0 || velo.Y != 0)
			isMoving = true;
		else
			isMoving = false;
	}

	cowboy = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (cowboy) {
		// keep updating the destination every tick while presed
		if (bMoveToMouseCursor && !cowboy->dead)	{
			MoveToMouseCursor();
		}
		if (!isMoving && !cowboy->dead) {
			RotatePlayer();
		}
	}

	// reset skill CDs if time passed
	if (skillOneCD) {
		activeTimerSkillOne += deltaTime;
		if (activeTimerSkillOne >= breakSkillOne) {		// enable movement again after break time
			canMove = true;
		}
		if (activeTimerSkillOne >= timerSkillOne) {
			skillOneCD = false;
			activeTimerSkillOne = 0;
		}
	}
	if (skillTwoCD) {
		activeTimerSkillTwo += deltaTime;
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
		activeTimerSkillThree += deltaTime;
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
				cowboy = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
				if (cowboy) cowboy->animRunning = true;
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

// ## mouse functions

void ACowboynoutPlayerController::OnLeftMousePressed() {
	ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(GetCharacter());
	if (playerChar) {
		// click on enemy
		if (playerChar->hasTarget == 1 || isStationairy) {
			OnSkillOnePressed();
		}
		// click on usable item
		else if (playerChar->hasTarget == 2) {
			//DebugMsg("you clicked an info itam!", 3.f, FColor::White);
			if (!info) {
				if (wInfoW) 				{
					info = true;
					myInfoW = CreateWidget<UUserWidget>(this, wInfoW);
					if (myInfoW) {
						myInfoW->AddToViewport();
					}
					bShowMouseCursor = true;
				}
			}
			else {
				myInfoW->RemoveFromParent();
				info = false;
			}
		}
		// click on collectable item
		else if (playerChar->hasTarget == 3) {
			DebugMsg("you clicked a collectible itam!", 3.f, FColor::White);
		}
		else {
			OnSetDestinationPressed();
		}
	}
}

void ACowboynoutPlayerController::OnLeftMouseReleased() {
	OnSetDestinationReleased();
}

void ACowboynoutPlayerController::OnRightMousePressed() {
	ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(GetCharacter());
	if (playerChar) {
		OnSkillTwoPressed();
	}
}

void ACowboynoutPlayerController::OnRightMouseReleased() {

}

void ACowboynoutPlayerController::OnSkillOnePressed() {
		if (!moveOnly ){
			AController::StopMovement();
		
			if (skillOneCD || breakSkillTwo) {
			}
			else {
				RotatePlayer();
				SkillOne();
			}
			
		}

}

void ACowboynoutPlayerController::OnSkillOneReleased() {

}

// WiP :: not good, fix later
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

void ACowboynoutPlayerController::OnSimulateDamagePressed() {
	// increase chip ammount
	SimulateDamage();
}

void ACowboynoutPlayerController::SimulateDamage() {
	ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	playerChar->life -= 10.f;
}

void ACowboynoutPlayerController::OnSimulateDamageReleased() {

}

void ACowboynoutPlayerController::RotatePlayer() {
	// Get mouse position on screen
	float xMouse;
	float yMouse;
	GetMousePosition(xMouse, yMouse);

	// Get Character position on screen
	ACharacter* character = GetCharacter();
	if (character != NULL) {
		FVector charLoc = character->GetActorLocation();
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

		character->SetActorRotation(rot);
	}
	

}

void ACowboynoutPlayerController::SkillOne() {
	//DebugMsg("pewpew", displayTime, FColor::Green);
	if (!skillOneCD) {
		cowboy = Cast<ACowboynoutCharacter>(GetCharacter());
		if (cowboy) {
			cowboy->FireSkillOne();
			skillOneCD = true;		// set CD
		}
		else DebugMsg("cowboy nullptr", displayTime, FColor::Red);
	}
	else {
		DebugMsg("skill one on CD", displayTime, FColor::Red);
		return;
	}
	AController::StopMovement();						// stop movement
	
	
	// play sound at player location
}

void ACowboynoutPlayerController::SkillTwo() {
	//DebugMsg("pewpew²", displayTime, FColor::Green);
	if (!skillTwoCD) {
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
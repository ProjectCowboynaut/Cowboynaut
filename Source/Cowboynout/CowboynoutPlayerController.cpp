// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.


#include "CowboynoutPlayerController.h"
#include "Enemy.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Perception/AISense.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "CowboynoutCharacter.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Slate.h"

ACowboynoutPlayerController::ACowboynoutPlayerController() {
	// set controle method : 0 d3, 1 twins, swedish twins!
	controleMethod = 0;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	gunPosition = FVector(0.f, 15.f, 0.f);
	myLittlePawny = GetPawn();

	// ## var init
	info = false;
	displayTime = .5f;
	isStationairy = false;
	deathTimerNotSet = false;
	deathTimerFull = 10.f;
	deathTimer = 0;

	// break == "no movement time"; timer == cd on skill
	timerSkillOne = .1f;
	breakSkillOne = .0f;
	activeTimerSkillOne = .0f;
	skillOneCD = false;
	timerSkillTwo = .1f;
	breakSkillTwo = .0f;
	activeTimerSkillTwo = .0f;
	skillTwoCD = false;
	skillTwoTPCD = false;
	timerSkillThree = .1f;
	breakSkillThree = .0f;
	activeTimerSkillThree = .0f;
	skillThreeCD = false;
	
	moveOnly = false;
	canMove = true;
	isMoving = false;

	canTP = false;

	canUpgrade = false;

	// init player vars & refs (overwritten if used in BP)
}

void ACowboynoutPlayerController::SetupInputComponent() {
	
	FString loadedMapName = GetWorld()->GetMapName();
	if (CheckMap(loadedMapName)) {

		// set up gameplay key bindings
		Super::SetupInputComponent();
		
		InputComponent->BindAxis("MoveForward", this, &ACowboynoutPlayerController::MoveForward);
		InputComponent->BindAxis("MoveRight", this, &ACowboynoutPlayerController::MoveRight);

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

		InputComponent->BindAction("SpacePressed", IE_Pressed, this, &ACowboynoutPlayerController::OnSpacePressed);

		// simulate damage
		InputComponent->BindAction("SimulateDamage", IE_Pressed, this, &ACowboynoutPlayerController::OnSimulateDamagePressed);
		InputComponent->BindAction("SimulateDamage", IE_Released, this, &ACowboynoutPlayerController::OnSimulateDamageReleased);

		//UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, sightConfig->GetSenseImplementation(), GetControlledPawn());

		InputComponent->BindAction("LvlUpSkillOne", IE_Pressed, this, &ACowboynoutPlayerController::OnSkillOneLevelUp);
		InputComponent->BindAction("LvlUpSkillTwo", IE_Pressed, this, &ACowboynoutPlayerController::OnSkillOneLevelUp);

		InputComponent->BindAction("statDomage", IE_Pressed, this, &ACowboynoutPlayerController::PlusStatA);
		InputComponent->BindAction("statLeSpeed", IE_Pressed, this, &ACowboynoutPlayerController::PlusStatB);
		InputComponent->BindAction("statLaStamina", IE_Pressed, this, &ACowboynoutPlayerController::PlusStatC);

		InputComponent->BindAction("SkillOneLvlUp", IE_Pressed, this, &ACowboynoutPlayerController::OnSkillOneLevelUp);
		InputComponent->BindAction("SkillTwoLvlUp", IE_Pressed, this, &ACowboynoutPlayerController::OnSkillTwoLevelUp);
	}
	else {
		DebugMsg("no movement!" + GetWorld()->GetMapName(), 2, FColor::Red);
	}
}

void ACowboynoutPlayerController::Tick(float deltaTime) {
	Super::Tick(deltaTime);

	// new movement
	FString loadedMapName = GetWorld()->GetMapName();
	if (CheckMap(loadedMapName)) {
		RotatePlayer();
		WASDMove(deltaTime);
	}

	cowboy = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (cowboy && cowboy->dead) return;

	myLittlePawny = GetPawn();
	if (myLittlePawny) {
		if (MovementInput.X != 0 || MovementInput.Y != 0)
			isMoving = true;
		else
			isMoving = false;
	}
	// reset skill CDs if time passed
	if (skillOneCD) {
		activeTimerSkillOne += deltaTime;

		if (activeTimerSkillOne >= timerSkillOne) {
			skillOneCD = false;
			activeTimerSkillOne = 0;
		}
	}
	if (skillTwoCD) {
		activeTimerSkillTwo += deltaTime;
		
		if (activeTimerSkillTwo >= timerSkillTwo) {
			skillTwoCD = false;
			skillTwoTPCD = false;
			activeTimerSkillTwo = 0;
		}
	}
	if (skillThreeCD) {
		activeTimerSkillThree += deltaTime;

		if (activeTimerSkillThree >= timerSkillThree) {
			skillThreeCD = false;
			activeTimerSkillThree = 0;
		}
	}
	
	// end cd timer 
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, GetWorld()->GetMapName());
	
	if (endGame) {
		deathTimer += deltaTime;
		if (!deathTimerNotSet) {
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

	cowboy = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (cowboy && cowboy->dead) return;

	myLittlePawny = GetPawn();
	if (myLittlePawny) {
		FVector velo = myLittlePawny->GetVelocity();
		if (velo.X != 0 || velo.Y != 0)
			isMoving = true;
		else
			isMoving = false;
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

void ACowboynoutPlayerController::WASDMove(float deltaTime) {
	if (!MovementInput.IsZero()) {
		//Scale our movement input axis values by 100 units per second
		MovementInput = MovementInput.GetSafeNormal() *100.f;

		myLittlePawny = GetPawn();
		if (myLittlePawny) {
			FVector NewLocation;
			NewLocation += GetActorForwardVector() * MovementInput.X * deltaTime * characterMovementSpeed;
			NewLocation += GetActorRightVector() * MovementInput.Y * deltaTime * characterMovementSpeed;
			DebugMsg(FString::SanitizeFloat(NewLocation.X) + "," + FString::SanitizeFloat(NewLocation.Y), 2, FColor::White);
			myLittlePawny->AddMovementInput(NewLocation);
		}
	}

	//myLittlePawny = GetPawn();
	//if (myLittlePawny) {
	//	// Find movement direction
	//	const float ForwardValue = MovementInput.X;
	//	const float RightValue = MovementInput.Y;

	//	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	//	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	//	// Calculate  movement
	//	const FVector Movement = MoveDirection * characterMovementSpeed * deltaTime;
	//	DebugMsg(FString::SanitizeFloat(Movement.X) + "," + FString::SanitizeFloat(Movement.Y) + GetWorld()->GetMapName(), 2, FColor::White);

	//	// If non-zero size, move this actor
	//	if (Movement.SizeSquared() > 0.0f)
	//	{
	//		const FRotator NewRotation = Movement.Rotation();
	//		FHitResult Hit(1.f);
	//		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);

	//		if (Hit.IsValidBlockingHit())
	//		{
	//			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
	//			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D);
	//			RootComponent->MoveComponent(Deflection, NewRotation, true);
	//			//myLittlePawny->AddMovementInput()
	//		}
	//	}
	//}

}

void ACowboynoutPlayerController::MoveForward(float axisValue) {
	MovementInput.X = FMath::Clamp<float>(axisValue, -1.0f, 1.0f);
}

void ACowboynoutPlayerController::MoveRight(float axisValue) {
	MovementInput.Y = FMath::Clamp<float>(axisValue, -1.0f, 1.0f);
}

void ACowboynoutPlayerController::OnSkillOneLevelUp() {
	cowboy = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (cowboy) {
		cowboy->skillLvlOne++;
	}
}

void ACowboynoutPlayerController::OnSkillTwoLevelUp() {
	cowboy = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (cowboy) {
		cowboy->skillLvlTwo++;
	}
}

void ACowboynoutPlayerController::OnSetStationairyPressed() {
	isStationairy = true;
	AController::StopMovement();
}

void ACowboynoutPlayerController::OnSetStationairyReleased() {
	isStationairy = false;
}

// ## mouse functions

bool ACowboynoutPlayerController::CheckMap(FString mapName) {
	bool r = false;
	FString legitMapNames[] = { "MapSpaceGanzesLV", "UEDPIE_0_MapSpaceGanzesLV",
								"MapSpaceBjoerninger", "UEDPIE_0_MapSpaceBjoerninger", 
								"Test_Map_Ersin", "UEDPIE_0_Test_Map_Ersin", 
								"Test_Map_Maddin", "UEDPIE_0_Test_Map_Maddin",
								"BossMap", "UEDPIE_0_BossMap",		
								"MapSpaceMaxwell", "UEDPIE_0_MapSpaceMaxwell"
	};
	for(FString mapName : legitMapNames) {
		if (mapName == GetWorld()->GetMapName()) r = true;
	}
		
	return r;
}

void ACowboynoutPlayerController::OnLeftMousePressed() {
	
		ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(GetCharacter());
		if (playerChar) {
			OnSkillOnePressed();
			// click on usable item
			if (playerChar->hasTarget == 2) {
				//DebugMsg("you clicked an info itam!", 3.f, FColor::White);
				if (!info) {
					if (wInfoW)	{
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
		}
}

void ACowboynoutPlayerController::OnLeftMouseReleased() {

}

void ACowboynoutPlayerController::OnRightMousePressed() {
	ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(GetCharacter());
	if (playerChar) {
		OnSkillThreePressed();
	}
}

void ACowboynoutPlayerController::OnSpacePressed() {
	SkillTwo();
}

void ACowboynoutPlayerController::OnRightMouseReleased() {

}

void ACowboynoutPlayerController::OnSkillOnePressed() {
	AController::StopMovement();
	SkillOne();
}

void ACowboynoutPlayerController::OnSkillOneReleased() {

}

// WiP :: not good, fix later
void ACowboynoutPlayerController::OnSkillTwoPressed() {
	SkillTwo();
}

void ACowboynoutPlayerController::OnSkillTwoReleased() {

}

void ACowboynoutPlayerController::OnSkillThreePressed() {
	SkillThree();
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

	if (!skillTwoCD) {
		cowboy = Cast<ACowboynoutCharacter>(GetCharacter());
		if (cowboy) cowboy->FireSkillTwo();
	} 
	
}

void ACowboynoutPlayerController::SkillThree() {
	if (!skillThreeCD) {
		DebugMsg("skill three fired", displayTime, FColor::Yellow);

	}
	else {
		DebugMsg("skill three on CD", displayTime, FColor::Red);
		return;
	}

	// stop movement
	AController::StopMovement();
	skillThreeCD = true;	// set CD
}

void ACowboynoutPlayerController::MedPack() {
	cowboy = Cast<ACowboynoutCharacter>(GetCharacter());
	if (cowboy) {
		if (cowboy->medPacks > 0) {
			if (cowboy->life + 50 <= cowboy->lifeMax) {
				cowboy->life += 50;
				cowboy->medPacks--;
			}
			else {
				cowboy->life = cowboy->lifeMax;
				cowboy->medPacks--;
			}
		}
		else {
			DebugMsg("you have no medpacks, fewl!!", displayTime, FColor::Red);
		}
	}
}


// bugs bunny
void ACowboynoutPlayerController::DebugMsg(FString msg, float dTime, FColor clr) {
	GEngine->AddOnScreenDebugMessage(-1, dTime, clr, msg);
}

void ACowboynoutPlayerController::PlusStatA() {
	ACowboynoutCharacter* cowboy = Cast<ACowboynoutCharacter>(GetCharacter());
	cowboy->ConvertChipStatA();
}

void ACowboynoutPlayerController::PlusStatB() {
	ACowboynoutCharacter* cowboy = Cast<ACowboynoutCharacter>(GetCharacter());
	cowboy->ConvertChipStatB();
}

void ACowboynoutPlayerController::PlusStatC() {
	ACowboynoutCharacter* cowboy = Cast<ACowboynoutCharacter>(GetCharacter());
	cowboy->ConvertChipStatC();
}
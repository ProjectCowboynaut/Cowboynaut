// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "CowboynoutGameMode.h"
#include "CowboynoutPlayerController.h"
#include "CowboynoutCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACowboynoutGameMode::ACowboynoutGameMode()
{

	// use our custom PlayerController class
	PlayerControllerClass = ACowboynoutPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Characters/BP_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
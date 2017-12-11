// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "CowboynoutCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Enemy.h"
#include "CowboynoutPlayerController.h"


ACowboynoutCharacter::ACowboynoutCharacter() {
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	muzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	muzzleLocation->SetupAttachment(RootComponent);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// ## var init
	targetString = "";
	dead = false;
	hasTarget = 0;
	deathTimer = 2.6f;
	deathTimerFull = deathTimer;
	hasTarget = 0;
	life = 100;
	lifeMax = 100;
	speed = 1;
	attack = 1;
	skillLvlOne = 1;
	skillLvlTwo = 1;
	chipsA = 0;
	chipsB = 0;
	chipsC = 0;
}

// set if the player has a target
void ACowboynoutCharacter::SetTarget(int targetStatus) {			/// 0: no target; 1: enemy target; 2: usable
	if (targetStatus == 0) {
		hasTarget = 0;
	}
	else if (targetStatus == 1) {
		hasTarget = 1;
	}
	else if (targetStatus == 2) {
		hasTarget = 2;
	}
	else hasTarget = 0;
}

void ACowboynoutCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		APlayerController* playerCtrl = Cast<APlayerController>(GetController());
		if (playerCtrl)
		{
			FHitResult TraceHitResult;
			playerCtrl->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}


	if (dead) {
		if (deathTimer > 0) {
			deathTimer -= DeltaSeconds;
			FString msg = FString::SanitizeFloat(deathTimerFull - (deathTimerFull - deathTimer));
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, msg);
		}
		if (deathTimer <= 0)
			UGameplayStatics::OpenLevel(this, TEXT("/Game/Maps/DeathScreen"), false);
	}
}

void ACowboynoutCharacter::Damage(int dmg) {
	life -= dmg;
	if (life <= 0) {
		Die();
		animDead = true;
	}
}



float ACowboynoutCharacter::GetHealth() {
	float healthReturn = life / lifeMax;
	return healthReturn;
}

void ACowboynoutCharacter::ConvertChipStatA() {
	if (chipsA >= 5) {
		chipsA -= 5;
		lifeMax += 10;
		life = lifeMax;
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "not enough chips to convert to stat A");
}

void ACowboynoutCharacter::Die() {
	dead = true;
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "ouch. dead man walking!");
	animDying = true;
}

void ACowboynoutCharacter::ConvertChipStatB() {
	if (chipsB >= 5) {
		chipsB -= 5;
		speed++;
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "not enough chips to convert to stat B");
}

void ACowboynoutCharacter::ConvertChipStatC() {
	if (chipsC >= 5) {
		chipsC -= 5;
		attack += 1;
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "not enough chips to convert to stat C");
}

void ACowboynoutCharacter::ConvertChipSkillA() {
	if (chipsA >= 10 && chipsB >= 10) {
		chipsA -= 10;
		chipsB -= 10;
		skillLvlOne++;
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "not enough chips to convert to skill A");
}

void ACowboynoutCharacter::ConvertChipSkillB() {
	if (chipsB >= 10 && chipsC >= 10) {
		chipsB -= 10;
		chipsC -= 10;
		skillLvlTwo++;
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "not enough chips to convert to skill B");
}

int ACowboynoutCharacter::GetChipsA() {
	return chipsA;
}

int ACowboynoutCharacter::GetChipsB() {
	return chipsB;
}

int ACowboynoutCharacter::GetChipsC() {
	return chipsC;
}

void ACowboynoutCharacter::FireSkillOne() {
	ACowboynoutPlayerController* playerCtrl = Cast<ACowboynoutPlayerController>(GetController());
	if (playerCtrl) {
		animShooting = true;
		//if (hasTarget || PC->isStationairy) {
		FRotator rot = GetActorRotation();
		FActorSpawnParameters spawnInfo;
		AProjectile* bullet = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, muzzleLocation->GetComponentLocation(), rot, spawnInfo);
		//}
	}
}

void ACowboynoutCharacter::FireSkillTwo() {
	animShooting = true;
	ACowboynoutPlayerController* playerCtrl = Cast<ACowboynoutPlayerController>(GetController());
	if (playerCtrl) {
		//if (hasTarget || PC->isStationairy) {
			FRotator rot = GetActorRotation();
			rot.Pitch = 60.f;
			FActorSpawnParameters spawnInfo;
			AGrenade* nade = GetWorld()->SpawnActor<AGrenade>(GrenadeClass, muzzleLocation->GetComponentLocation(), rot, spawnInfo);
		//}
	}
}

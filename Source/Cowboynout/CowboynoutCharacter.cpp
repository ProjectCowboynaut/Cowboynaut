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
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

// set if the player has a target
void ACowboynoutCharacter::SetTarget(bool targetStatus) {
	if (targetStatus == true) {
		hasTarget = true;
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, "Target Aquired!");
	}
	else if (targetStatus == false) {
		hasTarget = false;
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "Target Lost!");
	}
}

void ACowboynoutCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
}

void ACowboynoutCharacter::Damage(int dmg) {

}

void ACowboynoutCharacter::FireSkillOne() {
	//if (hasTarget) {
		FRotator rot = GetActorRotation();
		//DebugMsg(rot.ToString(), 1.f, FColor::White);
		FActorSpawnParameters spawnInfo;
		AProjectile* bullet = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, muzzleLocation->GetComponentLocation(), rot, spawnInfo);
		//if (bullet) DebugMsg("skill one fired", 1.f, FColor::Yellow);
		//else DebugMsg("404, bullet not found", 1.f, FColor::Red);
	//}
}

void ACowboynoutCharacter::FireSkillTwo() {
	FRotator rot = GetActorRotation();
	rot.Pitch = 60.f;
	//DebugMsg(rot.ToString(), 1.f, FColor::White);
	FActorSpawnParameters spawnInfo;
	AGrenade* nade = GetWorld()->SpawnActor<AGrenade>(GrenadeClass, muzzleLocation->GetComponentLocation(), rot, spawnInfo);
	//if (nade) DebugMsg("skill two fired", 1.f, FColor::Yellow);
	//else DebugMsg("404, no fire in the hole", 1.f, FColor::Red);
}
// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Grenade.h"
#include "CowboynoutCharacter.h"
#include "Enemy.h"
#include "CowboynoutPlayerController.h"
#include "GameFramework/ProjectileMovementComponent.h"

AGrenade::AGrenade() {
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AGrenade::OnHit);		// set up a notification for when this component hits something blocking

	CollisionComp->SetWalkableSlopeOverride(
		FWalkableSlopeOverride(
			WalkableSlope_Unwalkable, 0.f
		)
	); // Players can't walk on it

	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 800.0f;
	ProjectileMovement->MaxSpeed = 900.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->ProjectileGravityScale = .7f;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
	grenadeDamage = 100.f;

	cnt = 0;
}

void AGrenade::DebugMsg(FString msg, float dTime, FColor clr) {
	GEngine->AddOnScreenDebugMessage(-1, dTime, clr, msg);
}

void AGrenade::Initialize(int damage)
{
	grenadeDamage = damage;

}

void AGrenade::OnHit(UPrimitiveComponent* HitComp,
					AActor* OtherActor,
					UPrimitiveComponent* OtherComp,
					FVector NormalImpulse,
					const FHitResult& Hit)
{

	if (OtherActor != NULL) {
		//DebugMsg("BOOM goes " + OtherActor->GetFName().ToString(), 1.5f, FColor::Red);
		AEnemy* hitEnemy = Cast<AEnemy>(OtherActor);
		if (hitEnemy != NULL) hitEnemy->Damage(grenadeDamage);

		FHitResult cursorHit;
		APlayerController* pCtrl = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (pCtrl != NULL) pCtrl->GetHitResultUnderCursor(ECC_Visibility, false, cursorHit);
		
		float const distance = FVector::Dist(cursorHit.ImpactPoint, pCtrl->GetPawn()->GetActorLocation());
		if (distance) DebugMsg(FString::SanitizeFloat(distance), 1.5f, FColor::Yellow);
	}
	else {
		FString msg = "[skill 2] hit";
		FString hitObjectName = OtherActor->GetFName().ToString();
		//DebugMsg(msg + " " + hitObjectName, 1.5f, FColor::Yellow);
	}

	//ACowboynoutCharacter* hittedPlayer = Cast<ACowboynoutCharacter>(OtherActor);
	
	
	if (!ProjectileMovement->bShouldBounce || cnt > 3)
		Destroy();

	cnt++;

	UObject* WorldContextObject = GetWorld();
	FVector SpawnLocation = this->GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();

	UGameplayStatics::SpawnEmitterAtLocation(
		WorldContextObject,
		EmitterTemplate,
		SpawnLocation,
		SpawnRotation,
		true
	);
}
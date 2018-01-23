// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Grenade.h"
#include "CowboynoutCharacter.h"
#include "Enemy.h"
#include "CowboynoutPlayerController.h"
#include "Splosion.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/ProjectileMovementComponent.h"

AGrenade::AGrenade() {
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AGrenade::OnHit);		// set up a notification for when this component hits something blocking
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f)); 
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// set as root component
	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	InitialLifeSpan = 4.0f;
	grenadeDamage = 100.f;

	cnt = 0;

	ACowboynoutPlayerController* pCtrl = Cast<ACowboynoutPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (pCtrl) {
		pCtrl->GetHitResultUnderCursor(ECC_Visibility, false, cursorHit);
		cursorLoc = cursorHit.Location;
		
	}

}

void AGrenade::Tick(float DeltaTime) {
	actorLoc = GetActorLocation();
	actorLoc.Z = 0;
	cursorLoc.Z = 0;
	locDistance = FVector::Distance(actorLoc, cursorLoc);
	FString msg = "";

	UObject* WorldContextObject = GetWorld();
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();
	// spawn fx emitt0r
	UGameplayStatics::SpawnEmitterAtLocation(WorldContextObject, EmitterTemplate, SpawnLocation, SpawnRotation, true);
	FActorSpawnParameters spawnInfo;
	// spawn sploding actor 
	ASplosion* boomSphere = GetWorld()->SpawnActor<ASplosion>(SploderClass, SpawnLocation, SpawnRotation, spawnInfo);

	Destroy();
}

void AGrenade::DebugMsg(FString msg, float dTime, FColor clr) {
	GEngine->AddOnScreenDebugMessage(-1, dTime, clr, msg);
}

void AGrenade::Initialize(int damage) {

	grenadeDamage = damage;
}

void AGrenade::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if (OtherActor != NULL) {
		// dmg should be inflicted by emitter later, not by this projectile
		//AEnemy* hitEnemy = Cast<AEnemy>(OtherActor);
		//if (hitEnemy != NULL) hitEnemy->Damage(grenadeDamage);

		FHitResult cursorHit;
		APlayerController* pCtrl = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (pCtrl != NULL) pCtrl->GetHitResultUnderCursor(ECC_Visibility, false, cursorHit);

		float const distance = FVector::Dist(cursorHit.ImpactPoint, pCtrl->GetPawn()->GetActorLocation());
		if (distance) DebugMsg(FString::SanitizeFloat(distance), 1.5f, FColor::Yellow);
	}
	else {
		FString msg = "[skill 2] hit";
		FString hitObjectName = OtherActor->GetFName().ToString();
	}

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



void AGrenade::SpawnEmitter() {
	
}
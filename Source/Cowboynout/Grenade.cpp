// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Grenade.h"
#include "GameFramework/ProjectileMovementComponent.h"

AGrenade::AGrenade() {
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AGrenade::OnHit);		// set up a notification for when this component hits something blocking

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f)); // Players can't walk on it
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

}


void AGrenade::DebugMsg(FString msg, float dTime, FColor clr) {
	GEngine->AddOnScreenDebugMessage(-1, dTime, clr, msg);
}


void AGrenade::Initialize(int damage)
{
	grenadeDamage = damage;

}


void AGrenade::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FString msg = "[skill 2] hit";
	FString hitObjectName = OtherActor->GetFName().ToString();
	DebugMsg(msg + " " + hitObjectName, 1.5f, FColor::Yellow);

	ACowboynoutCharacter* hittedPlayer = Cast<ACowboynoutCharacter>(OtherActor);
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && Role == ROLE_Authority)
	{
		ProjectileMovement->bShouldBounce = false;
		Destroy();
	}
	else if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 10.0f, GetActorLocation());
		ProjectileMovement->bShouldBounce = false;
		Destroy();
	}
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && hittedPlayer != NULL && Role == ROLE_Authority)
	{
		ProjectileMovement->bShouldBounce = false;
		//hitted other player
		hittedPlayer->Damage(grenadeDamage);

		Destroy();
	}
	if (!ProjectileMovement->bShouldBounce)
		Destroy();


	UObject* WorldContextObject = GetWorld();
	FVector SpawnLocation = this->GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();;
	
	UGameplayStatics::SpawnEmitterAtLocation(WorldContextObject, EmitterTemplate, SpawnLocation, SpawnRotation, true);
	
}
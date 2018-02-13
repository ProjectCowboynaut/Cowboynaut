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
	//CollisionComp->OnComponentHit.AddDynamic(this, &AGrenade::OnHit);		// set up a notification for when this component hits something blocking
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGrenade::OnOverlapBegin);
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f)); 
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// set as root component
	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	InitialLifeSpan = 4.0f;
	grenadeDamage = 300.f;

	cnt = 0;

	// Die after x seconds by default
	InitialLifeSpan = .5f;

	hitEnemies = {};
}

void AGrenade::Tick(float DeltaTime) {


}

void AGrenade::DebugMsg(FString msg, float dTime, FColor clr)
{
	GEngine->AddOnScreenDebugMessage(-1, dTime, clr, msg);
}

void AGrenade::Initialize(int damage) 
{
	grenadeDamage = damage;
}


void AGrenade::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//DebugMsg("::" + OtherActor->GetFName().ToString() + "->" + OverlappedComp->GetFName().ToString(), 5.f, FColor::Yellow);
	if (OtherActor != nullptr) {

		// push box on hit
		if (OtherActor != this && OtherActor->GetFName().ToString() == "Box")
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 5.0f, GetActorLocation());
		}
		if (hitEnemies.Contains(OtherActor))
		{
			//DebugMsg("schon getroffen", 1.5f, FColor::Yellow);
		}
		else
		{
			hitEnemies.AddUnique(OtherActor);

			// >> hit enemy
			if ((OtherActor->ActorHasTag("Enemy") || OverlappedComp->ComponentHasTag("Enemy")))
			{
				//DebugMsg("p hit: " + OtherActor->GetName(), 1.5f, FColor::Red);
				// set dmg on enemy
				AEnemy* hitEnemy = Cast<AEnemy>(OtherActor);
				ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
				if (playerChar && hitEnemy)
				{
					hitEnemy->Damage(grenadeDamage * (1 + (playerChar->attack / 10)));
					//hitEnemy->PlaySound(0);
				}
			}
			else DebugMsg("no actor", 1.5f, FColor::Red);
		}
	}
	else DebugMsg("no actor", 1.5f, FColor::Red);

#pragma region old Nade
	////DebugMsg("::" + OtherActor->GetFName().ToString() + "->" + OverlappedComp->GetFName().ToString(), 5.f, FColor::Yellow);
	//if (OtherActor != nullptr) {

	//	// push box on hit
	//	if (OtherActor != this && OtherActor->GetFName().ToString() == "Box")
	//	{
	//		OtherComp->AddImpulseAtLocation(GetVelocity() * 5.0f, GetActorLocation());
	//	}

	//	if (hitEnemies.Contains(OtherActor))
	//	{
	//		//DebugMsg("schon getroffen", 1.5f, FColor::Yellow);
	//	}
	//	else
	//	{
	//		hitEnemies.AddUnique(OtherActor);

	//		// >> hit enemy
	//		if ((OtherActor->ActorHasTag("Enemy") || OverlappedComp->ComponentHasTag("Enemy")))
	//		{
	//			//DebugMsg("p hit: " + OtherActor->GetName(), 1.5f, FColor::Red);
	//			// set dmg on enemy
	//			AEnemy* hitEnemy = Cast<AEnemy>(OtherActor);
	//			ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//			if (playerChar && hitEnemy)
	//			{
	//				hitEnemy->Damage(grenadeDamage * (1 + (playerChar->attack / 10)));
	//				hitEnemy->PlaySound(0);
	//				// check for penetration and sub enemy armor
	//			}
	//		}
	//		
	//		else DebugMsg("no actor", 1.5f, FColor::Red);
	//	}
	//}
	//else DebugMsg("no actor", 1.5f, FColor::Red);
}
#pragma endregion old dmg used for "aoe tick"

void AGrenade::SpawnEmitter() {
	
}
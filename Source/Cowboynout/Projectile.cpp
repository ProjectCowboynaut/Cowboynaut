// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Projectile.h"
#include "CowboynoutCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Enemy.h"

AProjectile::AProjectile() {
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	if (playerChar) {
		ProjectileMovement->InitialSpeed = 3000.f * playerChar->speed;
		ProjectileMovement->MaxSpeed = 3000.f * playerChar->speed;
	}
	else {
		ProjectileMovement->InitialSpeed = 3000.f;
		ProjectileMovement->MaxSpeed = 3000.f;
	}
	
	ProjectileMovement->bRotationFollowsVelocity = true;
	if (playerChar) {
		if (playerChar->skillLvlOne == 1) {
			ProjectileMovement->bShouldBounce = false;
		}
		else if (playerChar->skillLvlOne == 2) {
			ProjectileMovement->bShouldBounce = true;
		}
		else if (playerChar->skillLvlOne == 3) {
			ProjectileMovement->bShouldBounce = true;
		}

	}
	ProjectileMovement->ProjectileGravityScale = 0.f;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	UObject* WorldContextObject = GetWorld();
	FVector SpawnLocation = this->GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();;

	// set projectile base stats depending on skill T
	if (playerChar) {
		if (playerChar->skillLvlOne == 1) {
			projectileDamage = 50 + (playerChar->attack * playerChar->attackGainPerLevel);
			penetration = 1;
		}
		else if (playerChar->skillLvlOne == 2) {
			projectileDamage = 60 + (playerChar->attack * playerChar->attackGainPerLevel);
			penetration = 2;
		}
		else if (playerChar->skillLvlOne == 3) {
			projectileDamage = 70 + (playerChar->attack * playerChar->attackGainPerLevel);
			penetration = 3;
		}
	}
}

void AProjectile::DebugMsg(FString msg, float dTime, FColor clr) {
	GEngine->AddOnScreenDebugMessage(-1, dTime, clr, msg);
}

void AProjectile::Initialize(int damage) {
	projectileDamage = damage;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if (OtherActor != NULL && OtherActor->GetFName().ToString() == "Box") {
		DebugMsg(OtherActor->GetFName().ToString(), 1.5f, FColor::Yellow);
		OtherComp->AddImpulseAtLocation(GetVelocity() * 5.0f, GetActorLocation());
	}

	if (OtherActor != NULL && !OtherActor->ActorHasTag("Player")) {
		ACowboynoutCharacter* hittedPlayer = Cast<ACowboynoutCharacter>(OtherActor);
		
		if (OtherActor->ActorHasTag("Enemy")) {
			// set dmg on enemy
			AEnemy* hitEnemy = Cast<AEnemy>(OtherActor);
			ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (playerChar && hitEnemy) {
				hitEnemy->Damage(projectileDamage * ( 1 + (playerChar->attack / 10)));
				hitEnemy->PlaySound(0);
				// check for penetration and sub enemy armor
				if (penetration - hitEnemy->armor <= 0) {
					Destroy();
					DebugMsg("*", 1.5f, FColor::Yellow);
				}
				else {
					penetration = penetration - hitEnemy->armor;
					DebugMsg("-1p", 1.5f, FColor::Yellow);
				}
			}
		}

		if (OtherActor->ActorHasTag("Destructible")) {
			DebugMsg("!", 1.5f, FColor::Red);
		}
		
		//// Only add impulse and destroy projectile if we hit a physics
		//else if ((OtherActor != this) && (OtherComp != NULL) && Role == ROLE_Authority)
		//{
		//	ProjectileMovement->bShouldBounce = false;
		//	Destroy();
		//}
		//else if ((OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
		//{
		//	OtherComp->AddImpulseAtLocation(GetVelocity() * 10.0f, GetActorLocation());
		//	ProjectileMovement->bShouldBounce = false;
		//	Destroy();
		//}
	}
	else if (OtherActor != NULL && OtherActor->ActorHasTag("Player")) {
		//DebugMsg("<!> pwned", 1.5f, FColor::Red);
		ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (playerChar) playerChar->Damage(projectileDamage);
	}
	else if (OtherActor != NULL) {
		//DebugMsg("<...>", 1.5f, FColor::Red);
	}


	if (!ProjectileMovement->bShouldBounce && !OtherActor->ActorHasTag("PlayerShot") && !OtherActor->ActorHasTag("EnemyShot")) {
		AEnemy* hitEnemy = Cast<AEnemy>(OtherActor);
		AActor* hitActor = Cast<AActor>(OtherActor);
		if (hitEnemy != NULL) {
			if (hitEnemies.Contains(hitActor)) {
				DebugMsg("schon getroffen", 1.5f, FColor::Yellow);
			}
			else {
				hitEnemies.AddUnique(hitActor);
				if (penetration - hitEnemy->armor <= 0) {
					Destroy();
					DebugMsg("*", 1.5f, FColor::Yellow);
				}
				else {
					penetration = penetration - hitEnemy->armor;
					DebugMsg("-1p", 1.5f, FColor::Yellow);
				}
			}
		}
		//else Destroy();
	}
}

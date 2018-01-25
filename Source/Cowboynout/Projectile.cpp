// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Projectile.h"
#include "CowboynoutCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Enemy.h"

AProjectile::AProjectile() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	debugEnabled = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	//CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;

	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	UObject* WorldContextObject = GetWorld();
	FVector SpawnLocation = this->GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();;

	bulletType = GetClass()->GetName();

	hitEnemies = {};

	// set PLAYER projectile base stats depending on skill T
	if (bulletType == "BP_SkillOne_T01_C" || bulletType == "BP_SkillOne_T02_C" || bulletType == "BP_SkillOne_T03_C"){
		playerProjectile = true;
		enemyProjectile = false;
		
		ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (playerChar) {
			ProjectileMovement->InitialSpeed = 4000.f * playerChar->speed;
			ProjectileMovement->MaxSpeed = 4000.f * playerChar->speed;
			if (playerChar->skillLvlOne == 1) {
				ProjectileMovement->bShouldBounce = false;
			}
			else if (playerChar->skillLvlOne == 2) {
				ProjectileMovement->bShouldBounce = false;
			}
			else if (playerChar->skillLvlOne == 3) {
				ProjectileMovement->bShouldBounce = false;
			}
		}
		else {
			ProjectileMovement->InitialSpeed = 3000.f;
			ProjectileMovement->MaxSpeed = 3000.f;
		}

		if (playerChar) {
			if (playerChar->skillLvlOne == 1) {
				projectileDamage = 50 + (playerChar->attack * playerChar->attackGainPerLevel);
				penetration = 5;
			}
			else if (playerChar->skillLvlOne == 2) {
				projectileDamage = 60 + (playerChar->attack * playerChar->attackGainPerLevel);
				penetration = 10;
			}
			else if (playerChar->skillLvlOne == 3) {
				projectileDamage = 70 + (playerChar->attack * playerChar->attackGainPerLevel);
				penetration = 15;
			}
		}
	}

	// set ENEMY projectile base stats
	else if (bulletType == "BP_Ememy_SkillOne_00_C" || bulletType == "BP_Enemy_SkillOne_01_C" || bulletType == "BP_EnemyProjectile_C") {
		enemyProjectile = true;
		playerProjectile = false;
		penetration = 1;
	}
}

void AProjectile::Tick(float deltaTime) {
	if (penetration <= 0) {
		//DebugMsg("xxx : pen", 1.5f, FColor::Red);
		Destroy();
	}
}

void AProjectile::DebugMsg(FString msg, float dTime, FColor clr) {
	if (debugEnabled)
		GEngine->AddOnScreenDebugMessage(-1, dTime, clr, msg);
}

void AProjectile::Initialize(int damage) {
	projectileDamage = damage;
}


void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor != NULL) {
		//DebugMsg(OtherActor->GetFName().ToString(), 1.5f, FColor::Yellow);
		// push box
		if (OtherActor != this && OtherActor->GetFName().ToString() == "Box") {
			OtherComp->AddImpulseAtLocation(GetVelocity() * 5.0f, GetActorLocation());
		}

		// >> hit !player
		if (hitEnemies.Contains(OtherActor)) {
			//DebugMsg("schon getroffen", 1.5f, FColor::Yellow);
		}
		else {
			hitEnemies.AddUnique(OtherActor);
			if ((OtherActor->ActorHasTag("Enemy") || OverlappedComp->ComponentHasTag("Enemy")) && playerProjectile) {
				//DebugMsg("p hit: " + OtherActor->GetName(), 1.5f, FColor::Red);
				// set dmg on enemy
				AEnemy* hitEnemy = Cast<AEnemy>(OtherActor);
				ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
				if (playerChar && hitEnemy) {
					hitEnemy->Damage(projectileDamage * (1 + (playerChar->attack / 10)));
					hitEnemy->PlaySound(0);
					// check for penetration and sub enemy armor
					penetration -= hitEnemy->armor;
				}
			}
			// >> hit player
			else if ((OtherActor->ActorHasTag("Player") || OverlappedComp->ComponentHasTag("Player")) && enemyProjectile) {
				ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
				if (playerChar) playerChar->Damage(projectileDamage);
				//DebugMsg("e hit: " + OtherActor->GetName(), 1.5f, FColor::Red);
				Destroy();
			}

		}

	}
	else DebugMsg("no actor" + OtherActor->GetName(), 1.5f, FColor::Red);

}


void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	//if (OtherActor != NULL && OtherActor->GetFName().ToString() == "Box") {
	//	DebugMsg(OtherActor->GetFName().ToString(), 1.5f, FColor::Yellow);
	//	OtherComp->AddImpulseAtLocation(GetVelocity() * 5.0f, GetActorLocation());
	//}

	//// >> hit @ drone
	//if (OtherActor != NULL && !OtherActor->ActorHasTag("Player")) {
	//	ACowboynoutCharacter* hittedPlayer = Cast<ACowboynoutCharacter>(OtherActor);
	//	
	//	if (OtherActor->ActorHasTag("Enemy") && playerProjectile) {
	//		// set dmg on enemy
	//		AEnemy* hitEnemy = Cast<AEnemy>(OtherActor);
	//		ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//		if (playerChar && hitEnemy) {
	//			hitEnemy->Damage(projectileDamage * ( 1 + (playerChar->attack / 10)));
	//			hitEnemy->PlaySound(0);
	//			// check for penetration and sub enemy armor
	//			if (penetration - hitEnemy->armor <= 0) {
	//				Destroy();
	//				DebugMsg("1", 1.5f, FColor::Yellow);
	//			}
	//			else {
	//				penetration = penetration - hitEnemy->armor;
	//				//DebugMsg("-1p", 1.5f, FColor::Yellow);
	//			}
	//		}
	//	}

	//	if (OtherActor->ActorHasTag("Destructible")) {
	//		//DebugMsg("!", 1.5f, FColor::Red);
	//	}
	//	
	//	//// Only add impulse and destroy projectile if we hit a physics
	//	//else if ((OtherActor != this) && (OtherComp != NULL) && Role == ROLE_Authority)
	//	//{
	//	//	ProjectileMovement->bShouldBounce = false;
	//	//	Destroy();
	//	//}
	//	//else if ((OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	//	//{
	//	//	OtherComp->AddImpulseAtLocation(GetVelocity() * 10.0f, GetActorLocation());
	//	//	ProjectileMovement->bShouldBounce = false;
	//	//	Destroy();
	//	//}
	//}
	//// >> hit @ the player
	//else if (OtherActor != NULL && OtherActor->ActorHasTag("Player")) {
	//	if (!this->ActorHasTag("PlayerShot")) {
	//		//DebugMsg("<!> pwned", 1.5f, FColor::Red);
	//		ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//		if (playerChar) playerChar->Damage(projectileDamage);
	//	}
	//}
	//else if (OtherActor != NULL) {
	//	//DebugMsg("<...>", 1.5f, FColor::Red);
	//}

	//if (!ProjectileMovement->bShouldBounce && !OtherActor->ActorHasTag("PlayerShot") && !OtherActor->ActorHasTag("EnemyShot")) {
	//	AEnemy* hitEnemy = Cast<AEnemy>(OtherActor);
	//	AActor* hitActor = Cast<AActor>(OtherActor);
	//	
	//	// player shot hitting player
	//	if (this->ActorHasTag("PlayerShot") && hitActor->ActorHasTag("Player")) {
	//		DebugMsg("PvP", 1.5f, FColor::Red);
	//	}
	//	// enemy shot hitting enemy
	//	else if (this->ActorHasTag("EnemyShot") && hitActor->ActorHasTag("Enemy")) {
	//		DebugMsg("EvE", 1.5f, FColor::Red);
	//	}
	//	else {
	//		if (hitEnemy != NULL) {
	//			if (hitEnemies.Contains(hitActor)) {
	//				//DebugMsg("schon getroffen", 1.5f, FColor::Yellow);
	//			}
	//			else {
	//				hitEnemies.AddUnique(hitActor);
	//				if (penetration - hitEnemy->armor <= 0) {
	//					Destroy();
	//					DebugMsg("0", 1.5f, FColor::Yellow);
	//				}
	//				else {
	//					penetration = penetration - hitEnemy->armor;
	//					//DebugMsg("-1p", 1.5f, FColor::Yellow);
	//				}
	//			}
	//		}
	//		else Destroy();
	//	}

	//}
}

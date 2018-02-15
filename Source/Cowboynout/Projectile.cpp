// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Projectile.h"
#include "CowboynoutCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Enemy.h"

AProjectile::AProjectile() 
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	statsSet = false;

	debugEnabled = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;

	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	// Die after 3 seconds by default
	InitialLifeSpan = 10.0f;

	UObject* WorldContextObject = GetWorld();
	FVector SpawnLocation = this->GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();;

	hitEnemies = {};
}

void AProjectile::Tick(float deltaTime) 
{
	//if (ProjectileMovement->Velocity.Size() <= 10.f) {
	//	Destroy();
	//}

	if (!statsSet) 
	{
		setBulletStats();
	}

	// (y) DebugMsg("<" + bulletType + ">", 5.f, FColor::Yellow);
	if (penetration <= 0) 
	{
		//DebugMsg("xxx : pen", 1.5f, FColor::Red);
		Destroy();
	}
}
 
void AProjectile::DebugMsg(FString msg, float dTime, FColor clr) 
{
	if (debugEnabled)
		GEngine->AddOnScreenDebugMessage(-1, dTime, clr, msg);
}

void AProjectile::Initialize(int damage) 
{
	//projectileDamage = damage;
}

void AProjectile::setBulletStats() 
{
	// set PLAYER projectile base stats depending on skill T
	if (bulletType == BulletType::PlayerBullet) 
	{
		playerProjectile = true;
		enemyProjectile = false;

		ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (playerChar) 
		{
			ProjectileMovement->InitialSpeed = 4000.f * playerChar->speed;
			ProjectileMovement->MaxSpeed = 4000.f * playerChar->speed;
			if (playerChar->skillLvlOne == 1) 
			{
				//DebugMsg("p1", 2.f, FColor::Red);
				ProjectileMovement->bShouldBounce = false;
				projectileDamage = 50 + (playerChar->attack * playerChar->attackGainPerLevel);
				penetration = 1;
			}
			else if (playerChar->skillLvlOne == 2) 
			{
				//DebugMsg("p2", 2.f, FColor::Red);
				ProjectileMovement->bShouldBounce = false;
				projectileDamage = 60 + (playerChar->attack * playerChar->attackGainPerLevel);
				penetration = 1;
			}
			else if (playerChar->skillLvlOne == 3) 
			{
				//DebugMsg("p3", 2.f, FColor::Red);
				ProjectileMovement->bShouldBounce = false;
				projectileDamage = 70 + (playerChar->attack * playerChar->attackGainPerLevel);
				penetration = 1;
			}
		}
		else 
		{
			//DebugMsg("?", 2.f, FColor::Red);
			//ProjectileMovement->InitialSpeed = 3000.f;
			//ProjectileMovement->MaxSpeed = 3000.f;
		}
	}

	// set ENEMY projectile base stats
	else if (bulletType == BulletType::EnemyBullet) 
	{
		//DebugMsg("e1", 2.f, FColor::Red);
		enemyProjectile = true;
		playerProjectile = false;
		ProjectileMovement->bShouldBounce = false;
		penetration = 1;
		//ProjectileMovement->InitialSpeed = 2000.f;
		//ProjectileMovement->MaxSpeed = 2000.f;
	}
	else 
	{
		DebugMsg("no bulletType found", 2.f, FColor::Red);
	}
	statsSet = true;
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	//DebugMsg("::" + OtherActor->GetFName().ToString() + "->" + OverlappedComp->GetFName().ToString(), 5.f, FColor::Yellow);
	if (OtherActor != nullptr){
		
		// push box on hit
		if (OtherActor != this && OtherActor->GetFName().ToString() == "Box") 
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 5.0f, GetActorLocation());
		}

		if (!OtherActor->ActorHasTag("Player") && hitEnemies.Contains(OtherActor))
		{
			//DebugMsg("schon getroffen", 1.5f, FColor::Yellow);
		}
		else 
		{
			hitEnemies.AddUnique(OtherActor);

			// >> hit enemy
			if ((OtherActor->ActorHasTag("Enemy") || OverlappedComp->ComponentHasTag("Enemy")) && playerProjectile) 
			{
				//DebugMsg("p hit: " + OtherActor->GetName(), 1.5f, FColor::Red);
				// set dmg on enemy
				AEnemy* hitEnemy = Cast<AEnemy>(OtherActor);
				ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
				if (playerChar && hitEnemy) 
				{
					hitEnemy->Damage(projectileDamage * (1 + (playerChar->attack / 10)));
					hitEnemy->PlaySound(0);
					// check for penetration and sub enemy armor
					Destroy();
				}
			}
			// >> hit player
			else if ((OtherActor->ActorHasTag("Player") || OverlappedComp->ComponentHasTag("Player")) && enemyProjectile) 
			{
				ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
				if (playerChar) playerChar->Damage(projectileDamage);
				//DebugMsg("e hit: " + OtherActor->GetName(), 1.5f, FColor::Red);
				Destroy();
			}
			else if (OtherActor->ActorHasTag("environment"))
			{
				Destroy();
			}
			else 
			{
				if (playerProjectile) Destroy();
				DebugMsg("no actor", 1.5f, FColor::Red);
			}
		}
	}
	else DebugMsg("no actor", 1.5f, FColor::Red);

}


void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) 
{

}

// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Loot.h"
//#include "Perception/AIPerceptionComponent.h"
#include "CowboynoutPlayerController.h"



// Sets default values
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	// var init
	chipsA = 0;
	chipsB = 0;
	chipsC = 0;
	behaviour = 0;							// 0 = go combat range, attack on sight; 1 = alert others; 2 = go close range, explode
	state = 0;								// 0 = passiv; 1 = allerted, 2 = attacking/triggered
	armor = 1;
	loopTime = 2.f;

	muzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	muzzleLocation->SetupAttachment(RootComponent);

	canSeePlayer = false;
}



void AEnemy::BeginPlay() {
	Super::BeginPlay();
	// trash mob stats
	if (type == 1) {
		health = 110;
		//attackRatio = 3.f;					// faster attacks, less dmg
		shieldOneActive = false;
		shieldOne = 0.f;
		shieldTwoActive = false;
		shieldTwo = 0.f;
		shieldThreeActive = false;
		shieldThree = 0.f;
		shieldFourActive = false;
		shieldFour = 0.f;
	}
	// boss mob stats
	else if (type == 666) {
		health = 510;
		//attackRatio = 5.f;					// lower ratio, more dmg per shot
		shieldOneActive = true;
		shieldOne = 200.f;
		shieldTwoActive = true;
		shieldTwo = 200.f;
		shieldThreeActive = true;
		shieldThree = 200.f;
		shieldFourActive = true;
		shieldFour = 200.f;
	}
	else {
		health = 110;
		//attackRatio = 3.f;
		shieldOneActive = false;
		shieldOne = 0.f;
		shieldTwoActive = false;
		shieldTwo = 0.f;
		shieldThreeActive = false;
		shieldThree = 0.f;
		shieldFourActive = false;
		shieldFour = 0.f;
	}

	//Timer handler
	FTimerHandle TimerHandle;

	//Activate the Timer
	//1st Parameter: TimerHandle
	//2nd Parameter: The Object related to this timer
	//3rd Parameter: The function that is going to be fired
	//4th Parameter: The loop time
	//5th Parameter: True - if you want this timer to loop, false otherwise
	// GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnemy::DoAPeriodicCheck, loopTime, true);
}

void AEnemy::Tick(float deltaTime) {
	Super::Tick(deltaTime);
	if (timerActive)
		timer += deltaTime;
	if (timer > attackRatio) {
		timerActive = false;
		timer = 0;
	}

	if (health <= 0) {
		Die();
	}

	if (deathTimerActive) {
		ACowboynoutPlayerController* Ctrl = Cast<ACowboynoutPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (playerChar) {
			Ctrl->endGame = true;
			Ctrl->deathTimerFull = Ctrl->endCD;
		}
	}
}

void AEnemy::SenseStuff(TArray<AActor*> testActors) {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "I see you!");
}

void AEnemy::SetNewMoveDestination(const FVector DestLocation) {

}

void AEnemy::RotateCharacter(const FVector DestLocation) {
	FRotator rot = FRotationMatrix::MakeFromX(GetActorLocation() - DestLocation).Rotator();
	//SetActorLocation(actorLoc);
	SetActorRotation(rot);
}

void AEnemy::Die() {
	ULoot* loot = AActor::FindComponentByClass<ULoot>();
	if (loot) {
		loot->DropChance(GetActorLocation(), this);
	}

	playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (playerChar) {
		playerChar->SetTarget(0);
		playerChar->targetString = "";
	}

	if (type == 666) {
		deathTimerActive = true;
	}

	Destroy();
}

void AEnemy::DoAPeriodicCheck() {
	FString msg = this->GetName();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, msg + ": Periodic Check has fired!");
	//GLog->Log("Periodic Check has fired!");
}

void AEnemy::Attack() {
	timerActive = true;
	FRotator rot = GetActorRotation();
	FActorSpawnParameters spawnInfo;
	AProjectile* bullet = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, muzzleLocation->GetComponentLocation(), rot, spawnInfo);
	if (bullet) PlaySound(1);
}

void AEnemy::MouseOverBegin(UPrimitiveComponent* TouchedComponent) {
	playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FString hitObjectName = TouchedComponent->GetFName().ToString();
	if (playerChar) {
		playerChar->SetTarget(1);
		playerChar->targetString = hitObjectName;
	}
}

void AEnemy::MouseOverEnd() {
	playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (playerChar) {
		playerChar->SetTarget(0);
		playerChar->targetString = "";
	}
}

void AEnemy::DestroyShield() {
	if (shieldFourActive) {
		shieldFour = 0;
		shieldFourActive = false;
	}
	else if (shieldThreeActive) {
		shieldThree = 0;
		shieldThreeActive = false;
	}
	else if (shieldTwoActive) {
		shieldTwo = 0;
		shieldTwoActive = false;
	}
	else if (shieldOneActive) {
		shieldOne = 0;
		shieldOneActive = false;
	}
}

void AEnemy::Damage(int dmg) {
	if (type == 1) {
		health -= dmg;
	}
	else if (type == 666) {
		if (shieldFourActive) {
			if (shieldFour > dmg) {
				shieldFour -= dmg;
			}
			else {
				shieldFour = 0;
				shieldFourActive = false;
			}
		}
		else if (shieldThreeActive) {
			if (shieldThree > dmg) {
				shieldThree -= dmg;
			}
			else {
				shieldThree = 0;
				shieldThreeActive = false;
			}
		}
		else if (shieldTwoActive) {
			if (shieldTwo > dmg) {
				shieldTwo -= dmg;
			}
			else {
				shieldTwo = 0;
				shieldTwoActive = false;
			}
		}
		else if (shieldOneActive) {
			if (shieldOne > dmg) {
				shieldOne -= dmg;
			}
			else {
				shieldOne = 0;
				shieldOneActive = false;
			}
		}
		else {
			health -= dmg;
		}
		
	}
	else{
		health -= dmg;
	}
}


// 0: been hit; 1: attack; 
void AEnemy::PlaySound(int sound) {
	float volumeMultiplier = 1.f;
	float pitchMultiplier = 1.f;
	float startTime = 0.f;

	UObject* worldContextObject = GetWorld();
	if (sound == 1) {
		int rnd = FMath::RandRange(0, 3);
		if (rnd == 1)
			UGameplayStatics::PlaySound2D(worldContextObject, soundSkill1a, volumeMultiplier, pitchMultiplier, startTime);
		else if (rnd == 2)
			UGameplayStatics::PlaySound2D(worldContextObject, soundSkill1b, volumeMultiplier, pitchMultiplier, startTime);
		else if (rnd == 3)
			UGameplayStatics::PlaySound2D(worldContextObject, soundSkill1c, volumeMultiplier, pitchMultiplier, startTime);
	}
		
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Loot.h"
#include "Perception/AIPerceptionComponent.h"



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
	armor = 0;

	loopTime = 2.f;

	muzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	muzzleLocation->SetupAttachment(RootComponent);

	canSeePlayer = false;
}



void AEnemy::BeginPlay() {
	Super::BeginPlay();
	// trash mob stats
	if (type == 1) {
		health = 100;
		attackRatio = 3.f;					// faster attacks, less dmg
	}
	// boss mob stats
	else if (type == 666) {
		health = 500;
		attackRatio = 5.f;					// lower ratio, more dmg per shot
	}
	else {
		health = 100;
		attackRatio = 3.f;
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
	FString msg = FString::SanitizeFloat(timer);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, msg);

	if (health <= 0) {
		Die();
	}
}

void AEnemy::SenseStuff(TArray<AActor*> testActors)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "I see you!");
}

void AEnemy::SetNewMoveDestination(const FVector DestLocation) {

}

void AEnemy::RotateCharacter(const FVector DestLocation) {
	FRotator rot = FRotationMatrix::MakeFromX(GetActorLocation() - DestLocation).Rotator();
	//SetActorLocation(actorLoc);
	SetActorRotation(rot);
}

void AEnemy::MouseOverBegin(UPrimitiveComponent* TouchedComponent) {
	playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (playerChar) playerChar->SetTarget(true);
	FString msg = "[mouse over begin]";
	FString hitObjectName = TouchedComponent->GetFName().ToString();
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, msg + " " + hitObjectName);
}

void AEnemy::Die() {
	ULoot* loot = AActor::FindComponentByClass<ULoot>();
	if (loot) {
		loot->DropChance(GetActorLocation(), this);
	}

	playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (playerChar) playerChar->SetTarget(false);

	Destroy();
}

void AEnemy::AttackFrequence() {

}

void AEnemy::DoAPeriodicCheck()
{
	FString msg = this->GetName();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, msg + ": Periodic Check has fired!");
	//GLog->Log("Periodic Check has fired!");
}

void AEnemy::Attack() {
	if (AEnemy* eChar = Cast<AEnemy>(GetController())) {
		FRotator rot = GetActorRotation();
		FActorSpawnParameters spawnInfo;
		AProjectile* bullet = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, muzzleLocation->GetComponentLocation(), rot, spawnInfo);
	}
}

void AEnemy::MouseOverEnd() {
	playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (playerChar) playerChar->SetTarget(false);

	FString msg = "[mouse over end]";
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, msg);
}

void AEnemy::Damage(int dmg) {
	health -= dmg;
}


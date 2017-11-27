// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Loot.h"
#include "Perception/AIPerceptionComponent.h"



// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}



void AEnemy::BeginPlay() {
	Super::BeginPlay();

}

void AEnemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
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
	player = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (player) player->SetTarget(true);
	FString msg = "[mouse over begin]";
	FString hitObjectName = TouchedComponent->GetFName().ToString();
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, msg + " " + hitObjectName);
}

void AEnemy::Die() {
	ULoot* loot = AActor::FindComponentByClass<ULoot>();
	if (loot) {
		loot->DropChance(GetActorLocation());
	}

	Destroy();
}

void AEnemy::MouseOverEnd() {
	player = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (player) player->SetTarget(false);

	FString msg = "[mouse over end]";
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, msg);
}

void AEnemy::Damage(int dmg) {
	health -= dmg;
}


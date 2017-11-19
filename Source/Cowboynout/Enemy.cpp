// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, player->GetFName().ToString());
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AEnemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AEnemy::SetNewMoveDestination(const FVector DestLocation) {
	
}

void AEnemy::RotateCharacter(const FVector DestLocation) {
	FRotator rot = FRotationMatrix::MakeFromX(GetActorLocation() - DestLocation).Rotator();
	//SetActorLocation(actorLoc);
	SetActorRotation(rot);
}

void AEnemy::MouseOverBegin(UPrimitiveComponent* TouchedComponent) {
	//player->hasTarget = true;
	//ACowboynoutCharacter::SetTarget(1);
	//player = Cast<ACowboynoutCharacter>(GetCharacter());
	if (player) player->SetTarget(1);

	FString msg = "[mouse over]";
	FString hitObjectName = TouchedComponent->GetFName().ToString();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, msg + " " + hitObjectName);
}

void AEnemy::MouseOverEnd() {
	
	if (player) player->SetTarget(0);
}
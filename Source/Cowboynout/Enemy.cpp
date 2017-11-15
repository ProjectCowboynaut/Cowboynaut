// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"


// Sets default values
AEnemy::AEnemy() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//TargetComponent = GetCapsuleComponent();
	
	TargetComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Test Component"));
	TargetComponent->AttachTo(RootComponent);

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TargetComponent->GetFName().ToString());

	TargetComponent->OnBeginCursorOver.AddDynamic(this, &AEnemy::CustomOnBeginMouseOver);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AEnemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::SetNewMoveDestination(const FVector DestLocation) {
	
}

void AEnemy::RotateCharacter(const FVector DestLocation) {
	FRotator rot = FRotationMatrix::MakeFromX(GetActorLocation() - DestLocation).Rotator();
	//SetActorLocation(actorLoc);
	SetActorRotation(rot);
}

void AEnemy::CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent){
	FString msg = "[mouse over]";
	FString hitObjectName = TouchedComponent->GetFName().ToString();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, msg + " " + hitObjectName);

}
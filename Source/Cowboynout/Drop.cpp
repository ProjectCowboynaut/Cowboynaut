// Fill out your copyright notice in the Description page of Project Settings.

#include "Drop.h"
#include "CowboynoutCharacter.h"
#include "Enemy.h"

// Sets default values
ADrop::ADrop(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(9.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Drop");
	CollisionComp->OnComponentHit.AddDynamic(this, &ADrop::OnHit);		// set up a notification for when this component hits something blocking
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));// Players can't walk on it
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ADrop::OnOverlap);

	// init type
	//type = 0;

	// Set as root component
	//RootComponent = CollisionComp;
}

// Called when the game starts or when spawned
void ADrop::BeginPlay(){
	Super::BeginPlay();
}

// Called every frame
void ADrop::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

// The method itself (Note the parameters)
void ADrop::OnOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){

	if (OtherActor != NULL && OtherActor->ActorHasTag("Player")) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, "MEINS!!");
		ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(OtherActor);
		// increase chip ammount
		if		(lootType == LootType::LootChipA) playerChar->chipsA++;
		else if (lootType == LootType::LootChipB) playerChar->chipsB++;
		else if (lootType == LootType::LootChipC) playerChar->chipsC++;
		else if (lootType == LootType::LootHealth) {
			if (playerChar->life + 50 <= playerChar->lifeMax) playerChar->life += 15;
			else playerChar->life = playerChar->lifeMax;
		}
		else if (lootType == LootType::LootNade) playerChar->nades++;
		// else
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, "lootus borkus maximus!!!");
		// destroy loot
		Destroy();
	}
}

void ADrop::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit){
	if (OtherActor != NULL && OtherActor->ActorHasTag("Player")) {
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, "meins!");
	}
}
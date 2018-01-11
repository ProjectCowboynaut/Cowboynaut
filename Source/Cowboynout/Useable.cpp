// Fill out your copyright notice in the Description page of Project Settings.

#include "Useable.h"
#include "CowboynoutPlayerController.h"

AUseable::AUseable() {
	PrimaryActorTick.bCanEverTick = true;
}

void AUseable::BeginPlay() {
	Super::BeginPlay();
}

void AUseable::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AUseable::AllowUpgrade(int state) {
	if (state == 0) {
		ACowboynoutPlayerController* pCtrl = Cast<ACowboynoutPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		pCtrl->canUpgrade = true;
	}
	else if (state == 1) {
		ACowboynoutPlayerController* pCtrl = Cast<ACowboynoutPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		pCtrl->canUpgrade = true;
	}
}

void AUseable::MouseOverBegin(UPrimitiveComponent* TouchedComponent) {
	isMouseOver = true;

	playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	//FString msg = "[mouse over begin]";
	FString hitObjectName = this->GetFName().ToString();

	if (playerChar){
		playerChar->SetTarget(2);
		//playerChar->target = GetParentActor();
		playerChar->targetString = hitObjectName;
	}
	
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, msg + " " + hitObjectName);
}

void AUseable::MouseOverEnd() {
	isMouseOver = false;
	
	playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (playerChar) {
		playerChar->SetTarget(0);
		playerChar->targetString = "";
	}
}
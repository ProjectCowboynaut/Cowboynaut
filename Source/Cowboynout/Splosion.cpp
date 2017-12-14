// you no take candle!

#include "Splosion.h"
#include "CowboynoutPlayerController.h"

// Sets default values
ASplosion::ASplosion() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	nadeTimer = 0;
	nadeTimerFull = 1.5f;

	ACowboynoutPlayerController* pCtrl = Cast<ACowboynoutPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (pCtrl) {
		pCtrl->canTP = true;
	}
	
	theEnd = false;
}

// Called when the game starts or when spawned
void ASplosion::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void ASplosion::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// > start timer
	nadeTimer += DeltaTime;
	ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (playerChar) {
		playerChar->nadeLoc = GetActorLocation();
	}
	// > timer end:
	if (nadeTimer >= nadeTimerFull) {
		//	TP not possible anymore
		// do dmg
		// spawn splosion actor
		ACowboynoutPlayerController* pCtrl = Cast<ACowboynoutPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (pCtrl) {
			pCtrl->canTP = false;
		}
		Destroy();
	}

	if (theEnd) {
		Destroy();
	}
}


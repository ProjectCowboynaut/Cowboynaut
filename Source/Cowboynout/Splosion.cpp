// you no take candle!

#include "Splosion.h"
#include "CowboynoutPlayerController.h"
#include "Enemy.h"

// Sets default values
ASplosion::ASplosion() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	nadeTimer = 0;
	nadeTimerFull = 1.5f;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(40.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Sploder");
	CollisionComp->OnComponentHit.AddDynamic(this, &ASplosion::OnHit);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASplosion::OnOverlapBegin);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ASplosion::OnOverlapEnd);
	
	//RootComponent = CollisionComp;

	ACowboynoutPlayerController* pCtrl = Cast<ACowboynoutPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (pCtrl) {
		pCtrl->canTP = true;
	}

	doHit = false;
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
		theEnd = playerChar->explodeNade;
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
		theEnd = true;
	}


	if (theEnd) {
		doHit = true;
		DoDmg(100);
	}
}

void ASplosion::DoDmg(int damage) {
	for (int32 Index = 0; Index != hitEnemies.Num(); ++Index) {
		// do dmg
		Cast<AEnemy>(hitEnemies[Index])->Damage(100);
	}
	// destroy orb
	Destroy();
}

void ASplosion::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if (OtherActor != NULL && OtherActor->ActorHasTag("Enemy")) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "hit");
		hitEnemies.AddUnique(OtherActor);
	}
}

void ASplosion::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor != NULL && OtherActor->ActorHasTag("Enemy")) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "o:b");
		hitEnemies.AddUnique(OtherActor);
	}
}

void ASplosion::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor != NULL && OtherActor->ActorHasTag("Enemy")) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "o:e");
		hitEnemies.Remove(OtherActor);
	}
}
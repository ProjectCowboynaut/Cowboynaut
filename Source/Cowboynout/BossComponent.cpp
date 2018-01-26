// you no take candle!

#include "BossComponent.h"
#include "Runtime/Core/Public/Math/UnrealMathVectorConstants.h"


UBossComponent::UBossComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UBossComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UBossComponent::BossFight() {

}

void UBossComponent::SpawnBullets(TSubclassOf<AProjectile> bulletBP, FVector center, float radius, int numberOfBulletsToFire, float bulletSpeed) {
	float rotation = 2 * PI / numberOfBulletsToFire;
	float d_rotation;
	FVector spawnPosi;

	// x + y calculation, lange bleibt gleich (r)
		for (int d = 0; d < numberOfBulletsToFire; d++)
		{
			d_rotation = d * rotation;
			spawnPosi.X = center.X + radius * FMath::Cos(d_rotation);
			spawnPosi.Y = center.Y + radius * FMath::Sin(d_rotation);
			spawnPosi.Z = center.Z;
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::SanitizeFloat(d_rotation));

			// spawn Projectile
			FRotator rot = FRotator(0,(d_rotation / PI * 180),0);
			FActorSpawnParameters spawnInfo;
			AProjectile* bossBullet = GetWorld()->SpawnActor<AProjectile>(bulletBP, spawnPosi, rot, spawnInfo);
			bossBullet->SetActorRotation(rot);
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, bulletBP->GetFName().ToString() + " fired");
		}
}

void UBossComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AEnemy* enemy = Cast<AEnemy>(this);
	if (enemy != nullptr) {
		if (enemy->bossFightActive) {

			//  ### during bossfight ###

			phaseTimerLive += DeltaTime;
			if (phaseTimerLive >= phaseTimerMax) {
				numberOfPhasesLive++;		// incrase phase number
				phaseTimerLive = 0;			// reset phase timer

			}

			BossFight();
		}
	}
	

}


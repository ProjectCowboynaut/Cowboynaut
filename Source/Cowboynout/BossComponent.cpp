// you no take candle!

#include "BossComponent.h"
#include "Runtime/Core/Public/Math/UnrealMathVectorConstants.h"


UBossComponent::UBossComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	//PrimaryActorTick.bStartWithTickEnabled = true;
	rotationTicker = 0;
}

void UBossComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBossComponent::BossFight() 
{

}

void UBossComponent::SpawnBullets(TSubclassOf<AProjectile> bulletBP, FVector center, float radius, int numberOfBulletsToFire, float bulletSpeed, float bulletDamage, float rotationSpeed) 
{
	float rotation = 2 * PI / numberOfBulletsToFire;
	float d_rotation;
	FVector spawnPosi;

	int angle = (int)rotationTicker % 360;
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::FromInt(angle));
	// x + y calculation, lange bleibt gleich (r)
	for (int d = 0; d < numberOfBulletsToFire; d++)
	{
		d_rotation = (d * rotation);
		spawnPosi.X = center.X + radius * FMath::Cos(d_rotation);
		spawnPosi.Y = center.Y + radius * FMath::Sin(d_rotation);
		spawnPosi.Z = center.Z;
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::SanitizeFloat(d_rotation));

		FRotator rot = FRotator(0, (d_rotation / PI * 180) + (angle * rotationSpeed), 0);
		FActorSpawnParameters spawnInfo;
		AProjectile* bossBullet = GetWorld()->SpawnActor<AProjectile>(bulletBP, spawnPosi, rot, spawnInfo);
		if (bossBullet)
		{
			bossBullet->SetActorRotation(rot);
			bossBullet->GetProjectileMovement()->InitialSpeed = bulletSpeed;
			bossBullet->GetProjectileMovement()->MaxSpeed = bulletSpeed;
			bossBullet->projectileDamage = bulletDamage;
			bossBullet->bulletType = BulletType::EnemyBullet;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, bulletBP->GetFName().ToString() + " fired");
	}
}

void UBossComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	rotationTicker += DeltaTime;

	//AEnemy* enemy = Cast<AEnemy>(this);
	//if (enemy != nullptr) {
	//	if (enemy->bossFightActive) {
			//  ### during bossfight ###
	phaseTimerLive += DeltaTime;
	if (phaseTimerMax == 0) phaseTimerMax = 5.f;
	if (phaseTimerLive >= phaseTimerMax) {
		numberOfPhasesLive++;		// incrase phase number
		phaseTimerLive = 0;			// reset phase timer
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::FromInt(numberOfPhasesLive));
	}
	if (numberOfPhasesLive > numberOfPhases) {
		numberOfPhasesLive = 0;
	}
	BossFight();
	//	}
	//}
}


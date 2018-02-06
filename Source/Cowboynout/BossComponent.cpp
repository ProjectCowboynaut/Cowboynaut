// you no take candle!

#include "BossComponent.h"
#include "Runtime/Core/Public/Math/UnrealMathVectorConstants.h"


UBossComponent::UBossComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	//PrimaryActorTick.bStartWithTickEnabled = true;
	rotationTicker = 0;
	numberOfTotalStateSwitches = (int) (100 / stateSwitchPercentage);
	boss = Cast<AEnemy>(this);
	stateSwitchesCount = 0;
	droneSpawnTimer = 0;
}

void UBossComponent::BossFight(float DeltaTime)
{
	if (!boss) boss = Cast<AEnemy>(this);
	else 
	{
		// switch states and increase state counter
		if (boss->health < (boss->healthMax / numberOfTotalStateSwitches * stateSwitchesCount))
		{
			stateSwitchesCount++;
			switch (bossState) {
				case BossState::BossIdle:
					bossState = BossState::BossAttack;
					break;
				case BossState::BossAttack:
					bossState = BossState::BossShield;
					break;
				case BossState::BossShield:
					bossState = BossState::BossAttack;
					break;
			}
		}
		// with spawn phase
		if (bossState == BossState::BossShield) {
			droneSpawnTimer += DeltaTime;
			if (droneSpawnTimer > droneSpawnTime) {
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "i be spawning!");
				
				FVector spawnPosi = boss->GetActorLocation();
				FRotator rot;
				FActorSpawnParameters spawnInfo;
				for (int i = 0; i < numberOfDronesToSpawnPerPhase; i++) {
					AEnemy* droneSpawn = GetWorld()->SpawnActor<AEnemy>(DroneBP, spawnPosi, rot, spawnInfo);
				}
			}
		}
		else if (bossState == BossState::BossAttack) {
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
		}
	}
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

	if (!boss) boss = Cast<AEnemy>(this);
	if (boss != nullptr) {
		if (boss->bossFightActive) {
			
			BossFight(DeltaTime);
		}
	}
}


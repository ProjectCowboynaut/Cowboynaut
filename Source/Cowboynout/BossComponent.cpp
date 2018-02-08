// you no take candle!

#include "BossComponent.h"
#include "Runtime/Core/Public/Math/UnrealMathVectorConstants.h"


UBossComponent::UBossComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	//PrimaryActorTick.bStartWithTickEnabled = true;
	rotationTicker = 0;

	boss = Cast<AEnemy>(this);
	stateSwitchesCount = 0;
	droneSpawnTimer = 0;
	healthForNextPhase = 0;
	numberOfDronesSpawned = 0;
}

void UBossComponent::BeginPlay()
{
	numberOfTotalStateSwitches = 100 / stateSwitchPercentage;
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::SanitizeFloat(stateSwitchPercentage));
	bossState = BossState::BossShield;
}

void UBossComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	rotationTicker += DeltaTime;

	if (!boss) boss = Cast<AEnemy>(this->GetOwner());
	if (boss != nullptr) {
		//if (boss->bossFightActive) {
		healthForNextPhase = (boss->healthMax / numberOfTotalStateSwitches) * (numberOfTotalStateSwitches - stateSwitchesCount);
		BossFight(DeltaTime);
		//}

	}
}

void UBossComponent::BossFight(float DeltaTime)
{
	if (!boss) boss = Cast<AEnemy>(this->GetOwner());
	else 
	{

		//// get array & read out stage vars from array
		//TArray<FString> stageInfo;
		//stages[stateSwitchesCount].ParseIntoArray(stageInfo, TEXT("; "));
		//// stageInfo >> [0]:#id; [1]:StageType; [2]:attacksToUse; [3]:droneAmmountToSpawn; [4]:droneSpawnTimer [5]:healthToSwitchStage; 
		//FString msg = "";
		//for (int i = 0; i < stageInfo.Num(); i++)
		//{
		//	msg += "::" + stageInfo[i];
		//}
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, msg);

		
		for (int i = 0; i < stages.Num(); i++)
		{
			stages[i].stageType;
			stages[i].healthPercentageToSwitchStage;
			stages[i].attackPatternsToUse;
			stages[i].dronesToSpawn;
			stages[i].spawnDuration;
			
		}


		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::SanitizeFloat(healthForNextPhase));
		if (bossState == BossState::BossShield) 
		{
			// spawn stage
			droneSpawnTimer += DeltaTime;
			
			if (droneSpawnTimer > droneSpawnTime && numberOfDronesSpawned < numberOfDronesToSpawnPerPhase)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "i be spawning!");
				droneSpawnTimer = 0;
				
				FVector spawnPosi = boss->GetActorLocation();
				spawnPosi += FVector(-1000, 0, 0);
				spawnPosi.Z = -840.876831f;
				FRotator rot;
				FActorSpawnParameters spawnInfo;

				AEnemy* droneSpawn = GetWorld()->SpawnActor<AEnemy>(DroneBP, spawnPosi, rot, spawnInfo);
				numberOfDronesSpawned++;
				if (droneSpawn)
				{
					droneSpawn->health = 200.f;
					droneSpawn->enemyType = EnemyType::EnemyBase;
				}	
			}

			if (numberOfDronesSpawned == numberOfDronesToSpawnPerPhase)
			{
				ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
				if (playerChar)
				{
					if (playerChar->enemiesActual == 1)
					{
						SwitchState();
					}
				}
			}
		}
		// attack stage
		else if (bossState == BossState::BossAttack) 
		{
			
			if (boss->health < healthForNextPhase)
			{
				SwitchState();
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "switching states");
			}
			phaseTimerLive += DeltaTime;
			if (phaseTimerMax == 0) phaseTimerMax = 5.f;
			if (phaseTimerLive >= phaseTimerMax) 
			{
				numberOfPhasesLive++;		// incrase phase number
				phaseTimerLive = 0;			// reset phase timer
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::FromInt(numberOfPhasesLive));
			}
			if (numberOfPhasesLive > numberOfPhases) 
			{
				numberOfPhasesLive = 0;
			}
		}
	}
}

void UBossComponent::SwitchState() 
{
	stateSwitchesCount++;
	numberOfDronesSpawned = 0;

	switch (bossState) 
	{
		case BossState::BossIdle:
			bossState = BossState::BossShield;
			break;
		case BossState::BossShield:
			bossState = BossState::BossAttack;
			break;
		case BossState::BossAttack:
			bossState = BossState::BossShield;
			break;
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

// you no take candle!

#include "BossComponent.h"
#include "Runtime/Core/Public/Math/UnrealMathVectorConstants.h"


UBossComponent::UBossComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	//PrimaryActorTick.bStartWithTickEnabled = true;
	rotationTicker = 0;

	boss = Cast<AEnemy>(GetOwner());
	stateSwitchesCount = 0;
	droneSpawnTimer = 0;
	healthForNextStage = 0;
	numberOfDronesSpawned = 0;
	phaseCtr = 0;
	shotTimer = 0;
	lastShotFired = 0;
	bossHealthMax = 0;
	if (boss) boss->bossDronesSpawnedThisPhase = 0;
		
}

void UBossComponent::BeginPlay()
{
	numberOfTotalStateSwitches = 100 / stateSwitchPercentage;
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::SanitizeFloat(stateSwitchPercentage));
	bossState = BossState::BossShield;
	if (!boss) boss = Cast<AEnemy>(GetOwner());
	if (boss != nullptr)
		bossHealthMax = boss->health;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), foundActors);	
}



void UBossComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	rotationTicker += DeltaTime;

	ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (playerChar) playerChar->dronesToSpawn = stages[stateSwitchesCount].dronesToSpawn;

	if (!boss) boss = Cast<AEnemy>(this->GetOwner());
	if (boss != nullptr) {
		if (bossHealthMax < boss->health || bossHealthMax == 0) bossHealthMax = boss->health;
		//if (boss->bossFightActive) {
		//healthForNextPhase = (boss->healthMax / numberOfTotalStateSwitches) * (numberOfTotalStateSwitches - stateSwitchesCount);
		BossFight(DeltaTime);
		//}

	}

	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), foundActors);
	//for (int i = 0; i < foundActors.Num(); i++)
	//{
	//	AEnemy* derGegna = Cast<AEnemy>(foundActors[i]);
	//	if (derGegna->enemyType == EnemyType::EnemyBossSpawn)
	//		if (bossDronesSpawnedThisPhase < bossDrones.Num()) 
	//			bossDronesSpawnedThisPhase = bossDrones.Num();
	//}
	
}

void UBossComponent::BossFight(float DeltaTime)
{
	if (!boss) boss = Cast<AEnemy>(GetOwner());
	else 
	{
		// ## read out vars from "array"
		actualStageType = stages[stateSwitchesCount].stageType;
		healthForNextStage = stages[stateSwitchesCount].healthPercentageToSwitchStage;  		// when to switch to next stage
		//stages[stateSwitchesCount].attackPatternsToUse;										// patterns to use in attackstage phases
		numberOfDronesToSpawnPerPhase = stages[stateSwitchesCount].dronesToSpawn;				// how many drones should be spawned during phase (kill all to end)
		stages[stateSwitchesCount].spawnFrequency;												// how long should drones be spawned

		if (bossState == BossState::BossShield) 
		{
			boss->bossDronesToSpawnThisPhase = stages[stateSwitchesCount].dronesToSpawn;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), foundActors);
			//GetBossDrones();
			ACowboynoutGameMode* gm = Cast<ACowboynoutGameMode>(GetWorld()->GetAuthGameMode());

			// spawn stage
			/*	droneSpawnTimer += DeltaTime;*/
			//droneSpawnTimer > droneSpawnTime
			if (boss->bossDronesSpawnedThisPhase <= numberOfDronesToSpawnPerPhase)
			{
				if (gm && !gm->bossIsSpawning) {
					gm->StartBossSpawn(0.f, .2f, 1.f, stages[stateSwitchesCount].spawnerList);
				}
			}
			if (boss && boss->bossDronesSpawnedThisPhase > numberOfDronesToSpawnPerPhase)
			{
				ACowboynoutGameMode* gm = Cast<ACowboynoutGameMode>(GetWorld()->GetAuthGameMode());
				gm->StopSpawn();
				ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
				if (playerChar)
				{
					if (boss->bossDrones.Num() <= 1)
					{
						// switch to next state in list
						SwitchState(stages[stateSwitchesCount+1].stageType);
					}
				}
			}
		}
		
		// attack stage
		else if (bossState == BossState::BossAttack) 
		{
			if (stages[stateSwitchesCount].attackPatterns[phaseCtr].bulletLifeTime != 0)
				float lifeTime = stages[stateSwitchesCount].attackPatterns[phaseCtr].bulletLifeTime != 0;


			lastShotFired += DeltaTime;
			// set life value for next trigger
			healthForNextStage = bossHealthMax * stages[stateSwitchesCount].healthPercentageToSwitchStage;

			shotTimer = stages[stateSwitchesCount].attackPatterns[phaseCtr].attackRate;

			if (lastShotFired > shotTimer)
			{
				if (phaseCtr == NULL) phaseCtr = 0;
				if (stateSwitchesCount == NULL) stateSwitchesCount = 0;

				SpawnBullets(	stages[stateSwitchesCount].attackPatterns[phaseCtr].bulletBP,
								stages[stateSwitchesCount].attackPatterns[phaseCtr].radius,
								stages[stateSwitchesCount].attackPatterns[phaseCtr].numberOfBulletsToFire,
								stages[stateSwitchesCount].attackPatterns[phaseCtr].bulletSpeed,
								stages[stateSwitchesCount].attackPatterns[phaseCtr].bulletDamage,
								DeltaTime,
								stages[stateSwitchesCount].attackPatterns[phaseCtr].attackRate,
								stages[stateSwitchesCount].attackPatterns[phaseCtr].bulletLifeTime
				);
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, stages[stateSwitchesCount].attackPatterns[phaseCtr].bulletBP->GetFName().ToString());
				lastShotFired = 0;
				//  phase counter increase or reset @max
				if (phaseCtr < stages[stateSwitchesCount].attackPatterns.Num()-1) 
					phaseCtr++;
				else phaseCtr = 0;
			}
			// switch when life triggered
			if (boss->health < healthForNextStage)
			{
				boss->vincible = false;
				SwitchState(stages[stateSwitchesCount + 1].stageType);
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "switching states");
			}

			// switch attacks
			phaseTimerLive += DeltaTime;
			if (phaseTimerMax == 0) phaseTimerMax = 5.f;
			if (phaseTimerLive >= phaseTimerMax) 
			{
				numberOfPhasesLive++;		// incrase phase number
				phaseTimerLive = 0;			// reset phase timer
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::FromInt(numberOfPhasesLive));
			}
			// reset phases to 0 if max reached
			if (numberOfPhasesLive > numberOfPhases) 
			{
				numberOfPhasesLive = 0;
			}
		}
	}
}

void UBossComponent::SwitchState(BossState state)
{
	if (boss->health <= 0) return;

	switch (state) 
	{
		case  BossState::BossShield:
			bossState = BossState::BossShield;
			if (boss) boss->bossDronesSpawnedThisPhase = 0;
			stateSwitchesCount++;
			numberOfDronesSpawned = 0;
			phaseCtr = 0;
			break;
		case BossState::BossAttack:
			bossState = BossState::BossAttack;
			if (boss) boss->bossDronesSpawnedThisPhase = 0;
			stateSwitchesCount++;
			numberOfDronesSpawned = 0;
			phaseCtr = 0;
			break;
		case BossState::BossRage:
			bossState = BossState::BossRage;
			if (boss) boss->bossDronesSpawnedThisPhase = 0;
			stateSwitchesCount++;
			numberOfDronesSpawned = 0;
			phaseCtr = 0;
			break;
	}
}

void UBossComponent::SpawnBullets(TSubclassOf<AProjectile> bulletBP, float radius, int numberOfBulletsToFire, float bulletSpeed, float bulletDamage, float deltaTime, float attackRate, float bulletLifeTime)
{
	// xy: world location actor, z: world location player
	FVector center;
	ACowboynoutCharacter* playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (!GetOwner()->GetActorLocation().IsZero()) 
	{
		FString msg = FVector(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y, playerChar->GetActorLocation().Z).ToString();
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, msg);
		if (playerChar)	center = FVector(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y, playerChar->GetActorLocation().Z);
		else center = FVector(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y, -700.f);
	}
	else center = FVector(0,0, -700.f);
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

		FRotator rot = FRotator(0, (d_rotation / PI * 180) + (angle * 100), 0);
		FActorSpawnParameters spawnInfo;
		AProjectile* bossBullet = GetWorld()->SpawnActor<AProjectile>(bulletBP, spawnPosi, rot, spawnInfo);
		if (bossBullet)
		{
			bossBullet->SetActorRotation(rot);
			bossBullet->GetProjectileMovement()->InitialSpeed = bulletSpeed;
			bossBullet->GetProjectileMovement()->MaxSpeed = bulletSpeed;
			bossBullet->projectileDamage = bulletDamage;
			bossBullet->bulletType = BulletType::EnemyBullet;
			bossBullet->InitialLifeSpan = bulletLifeTime;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, bulletBP->GetFName().ToString() + " fired");
	}
}

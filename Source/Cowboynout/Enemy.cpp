// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include <limits>
#include "Loot.h"
#include "BossComponent.h"
//#include "Perception/AIPerceptionComponent.h"
//#include "Runtime/AIModule/Classes/AIController.h"
#include "AAIAgent.h"
#include "CowboynoutPlayerController.h"

// Sets default values
AEnemy::AEnemy()
{
	healthBase = 200.f;
	healthElite = 400.f;
	healthBoss = 5000.f;
	attackRatioBase = .5f;
	attackRatioElite = .4f;
	attackRatioBoss = .5f;
	damageBase = 7.f;
	damageElite = 15.f;
	damageBoss = 20.f;


	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	// var init
	chipsA = 0;
	chipsB = 0;
	chipsC = 0;
	armor = 1;
	loopTime = 2.f;

	muzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	muzzleLocation->SetupAttachment(RootComponent);

	canSeePlayer = false;
	bossEffectTimer = 4.f;
	bossEffectTimerActive = 0.f;
	bossFightActive = false;				// set over BP

	isFriendly = false;
	isBoss = false;
	pillarsToActivate = 0;
	pillarsActive = 0;
	vincible = false;


	if (this->enemyType == EnemyType::EnemyBase)
	{
		/*this->health = healthBase;*/
		attackRatio = attackRatioBase;					// faster attacks, less dmg
		health = healthBase;
		armor = 1.f;
	}
	// elite mob stats
	else if (this->enemyType == EnemyType::EnemyElite)
	{
		/*this->health = healthElite;*/
		attackRatio = attackRatioElite;					// faster attacks, less dmg
		health = healthElite;
		armor = 1.f;
	}
	else if (this->enemyType == EnemyType::EnemyBossSpawn)
	{
		/*this->health = healthBoss;
		if (this->health == 0) health = 10000;*/
		attackRatio = attackRatioBase;					// lower ratio, more dmg per shot
		health = healthBase;
		armor = 1.f;
		isBoss = false;
	}
	// boss stats
	else if (this->enemyType == EnemyType::EnemyBoss)
	{
		/*this->health = healthBoss;
		if (this->health == 0) health = 10000;*/
		attackRatio = attackRatioBoss;					// lower ratio, more dmg per shot
		health = healthBoss;
		armor = 1.f;
		isBoss = true;
	}
	else 
	{
		/*this->health = 123;*/
		attackRatio = .2f;					// faster attacks, less dmg

	}

	healthMax = health;
}

void AEnemy::BeginPlay() 
{
	Super::BeginPlay();
	// trash mob stats
	

	// disable boss dmg area on start
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PillarBPClass, foundPillars);
	TArray<UActorComponent*> children;
	for (int z = 0; z < foundPillars.Num(); z++) 
	{
		if (foundPillars.IsValidIndex(z))
			foundPillars[z]->GetComponents(children);
		for (int i = 0; i < children.Num(); i++) 
		{
			if (children.IsValidIndex(i)) 
			{
				UActorComponent* child = children[i];
				FString name = child->GetName();
				USceneComponent* node = Cast<USceneComponent>(children[i]);
				if (child->GetName() == "BossAreaDamage") 
				{
					child->Deactivate();
					node->ToggleVisibility(true);
					//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "-d");
				}
				else if (child->GetName() == "BossAreaWarning") 
				{
					child->Deactivate();
					node->ToggleVisibility(true);
					//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "-w");
				}
			}
		}
	}
}

void AEnemy::Tick(float deltaTime) 
{
	Super::Tick(deltaTime);

	if (this->enemyType == EnemyType::EnemyBoss)
	{		
		bossHealth = health;
		if (bossHealth > bossHealthMax) bossHealthMax = bossHealth;
		int compare = bossDrones.Num();
		if (compare <= 1) vincible = true;
		else vincible = false;
		
	}
	if (health > healthMax) healthMax = health;

	// change state if all drones (but boss) are destroyed
	if (!playerChar) playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	

	if (timerActive)
		timer += deltaTime;
	if (timer > attackRatio) 
	{
		timerActive = false;
		timer = 0;
	}

	if (health <= 0) 
	{
		Die();
	}

	if (deathTimerActive) 
	{
		ACowboynoutPlayerController* pCtrl = Cast<ACowboynoutPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (playerChar) 
		{
			pCtrl->endGame = true;
			pCtrl->deathTimerFull = pCtrl->endCD;
		}
	}

	if (bossFightActive) 
	{
		BossFight(deltaTime);
	}

	// get action to do
	/*UAAIAgent* ai = AActor::FindComponentByClass<UAAIAgent>();
*/
	//// .1 get own health
	//evalInput.currentHealth = health;

	//// .2 get range to heal
	//TArray<AActor*> allEnemies;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), allEnemies);

	//AActor* selectedEnemy;
	//float rangeToHealer = FLT_MAX;

	//for (auto enemy : allEnemies) {
	//	if (Cast<AEnemy>(enemy)->type == 2 && GetDistanceTo(enemy) < rangeToHealer)	{
	//		this->evalInput.rangeToHealer = GetDistanceTo(enemy);
	//		selectedEnemy = enemy;
	//	}
	//}

	//// .3 get range to heal
	//this->evalInput.rangeToPlayer = GetDistanceTo(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	//this->action = ai->EvaluateActions(this->evalInput);
}

float AEnemy::GetBossHealth() {
	return bossHealth;
}

// old one, use the new one in bosscomponent
void AEnemy::BossFight(float deltaTime) 
{
	//if (isBoss) 
	//{
	//	UBossComponent* boss = AActor::FindComponentByClass<UBossComponent>();
	//	if (boss)
	//	{
	//		boss->BossFight(deltaTime);
	//	}
	//}
//	// pillars
//	bossEffectTimerActive += deltaTime;
//	if (bossEffectTimerActive >= bossEffectTimer) 
//	{
//		// > enable 2 plates
//		bossEffectTimerActive = 0;
//		bossEffectTimer = FMath::RandRange(1.f, 6.f);
//
//		UGameplayStatics::GetAllActorsOfClass(GetWorld(), PillarBPClass, foundPillars);
//		pillars = foundPillars.Num();
//
//#pragma region de-/activate half the pillars
//
//		pillarsToActivate = pillars / 2;
//		pillarsActive = 0;
//		//FString msg = "# of pillars to activate: ";
//		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, msg + FString::FromInt(pillarsToActivate));
//		pa = -1;
//		pillarsLastActive = -1;
//
//		while (pillarsActive < pillarsToActivate) 
//		{
//			while (pa == pillarsLastActive)
//				pa = FMath::RandRange(0, foundPillars.Num()-1);
//
//			//FString msg = "activating pillar: ";
//			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, msg + FString::FromInt(pa));
//			// -----
//			TArray<UActorComponent*> childComp;
//			//for (int p = 0; p < pillars; p++) {
//				// check if pillar is to activate or not
//				
//				foundPillars[pa]->GetComponents(childComp);
//				if (childComp.IsValidIndex(pa)) {
//					UActorComponent* child = childComp[pa];
//						
//					for (UActorComponent* c : childComp) {
//
//						FString name = c->GetName();
//						if (c->GetName() == "BossAreaDamage") 
//						{
//							USceneComponent* node = Cast<USceneComponent>(c);
//							if (c->IsActive()) 
//							{
//								c->Deactivate();
//								node->ToggleVisibility(true);
//								//FString msg = c->GetName() + " + " + FString::FromInt(pa) + "+";
//								//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, msg);
//							}
//							else 
//							{
//								c->Activate();
//								node->ToggleVisibility(true);
//								//FString msg = c->GetName() + " + " + FString::FromInt(pa) + "-";
//								//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, msg);
//							}
//						}
//						else if (c->GetName() == "BossAreaWarning") 
//						{
//							USceneComponent* node = Cast<USceneComponent>(c);
//							if (c->IsActive()) 
//							{
//								c->Deactivate();
//								node->ToggleVisibility(true);
//								//FString msg = c->GetName() + " + " + FString::FromInt(pa) + "+";
//								//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, msg);
//							}
//							else 
//							{
//								c->Activate();	
//								node->ToggleVisibility(true);
//								//FString msg = c->GetName() + " + " + FString::FromInt(pa) + "-";
//								//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, msg);
//							}
//						}
//						
//					}
//
//				}
//			//}
//			// -----
//			pillarsLastActive = pa;
//			pillarsActive++;
//		}	
//	}
//#pragma endregion
//
	
}

void AEnemy::SenseStuff(TArray<AActor*> testActors) 
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "I see you!");
}

void AEnemy::SetNewMoveDestination(const FVector DestLocation) 
{

}

void AEnemy::RotateCharacter(const FVector DestLocation) 
{
	FRotator rot = FRotationMatrix::MakeFromX(GetActorLocation() - DestLocation).Rotator();
	//SetActorLocation(actorLoc);
	SetActorRotation(rot);
}

void AEnemy::Die() 
{
	FVector lootLocation = this->GetActorLocation();
	lootLocation += FVector(0, 0, 100.f);
	ULoot* loot = AActor::FindComponentByClass<ULoot>();
	if (loot) 
	{
		loot->DropChance(lootLocation, this);
	}

	playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (playerChar) 
	{
		playerChar->SetTarget(0);
		playerChar->targetString = "";
	}
	ACowboynoutPlayerController* playerCtrl = Cast<ACowboynoutPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	// add score points
	if (playerCtrl) {
		if (enemyType == EnemyType::EnemyBase) playerCtrl->sessionScore += 10;
		else if (enemyType == EnemyType::EnemyElite) playerCtrl->sessionScore += 100;
		else if (enemyType == EnemyType::EnemyBoss) playerCtrl->sessionScore += 1000;
	}

	

	// if dead enemy == boss, start end
	if (enemyType == EnemyType::EnemyBoss) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, "Boss killed, the end is nigh");
		deathTimerActive = true;
	}
	UObject* worldContextObject = GetWorld();
	FVector spawnLocation = this->GetActorLocation();

	FActorSpawnParameters spawnInfo;
	AActor* borkedDrone = GetWorld()->SpawnActor<AActor>(brokenDroneMesh, spawnLocation, FRotator(90,0,0), spawnInfo);
	// Create a damage event  
	TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
	FDamageEvent DamageEvent(ValidDamageTypeClass);
	class AActor* DamageCauser = playerChar;
	if (borkedDrone) 
	{
		borkedDrone->TakeDamage(100, DamageEvent, UGameplayStatics::GetPlayerController(GetWorld(), 0), Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)));			// initialize dmg to break drone
		borkedDrone->InitialLifeSpan = 3.f;		// auto destroy after x secs
	}

	worldContextObject = GetWorld();
	spawnLocation = this->GetActorLocation();
	FRotator spawnRotation = this->GetActorRotation();

	UGameplayStatics::SpawnEmitterAtLocation(worldContextObject, dmgEffectParticle, spawnLocation, spawnRotation, true);

	if (enemyType == EnemyType::EnemyBossSpawn)
	{
		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), foundActors);
		for (int i = 0; i < foundActors.Num(); i++)
		{
			AEnemy* thisDrone = Cast<AEnemy>(foundActors[i]);
			if (thisDrone->enemyType == EnemyType::EnemyBoss)
			{
				thisDrone->bossDrones.Remove(this);
			}
		}
	}

	Destroy();
}

void AEnemy::DoAPeriodicCheck() 
{
	FString msg = this->GetName();
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, msg + ": Periodic Check has fired!");
	//GLog->Log("Periodic Check has fired!");
}

void AEnemy::Attack() 
{
	timerActive = true;
	FRotator rot = GetActorRotation();
	FActorSpawnParameters spawnInfo;
	AProjectile* bullet = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, muzzleLocation->GetComponentLocation(), rot, spawnInfo);
	if (bullet) {
		bullet->enemyProjectile = true;
		bullet->bulletType = BulletType::EnemyBullet;
		if		(enemyType == EnemyType::EnemyBase)			bullet->projectileDamage = damageBase;
		else if (enemyType == EnemyType::EnemyElite)		bullet->projectileDamage = damageElite;
		else if (enemyType == EnemyType::EnemyBossSpawn)	bullet->projectileDamage = damageBase;
		else if (enemyType == EnemyType::EnemyBoss)			bullet->projectileDamage = damageBoss;
		bullet->playerProjectile = false;
	}
	PlaySound(1);
}

void AEnemy::MouseOverBegin(UPrimitiveComponent* TouchedComponent) 
{
	playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FString hitObjectName = TouchedComponent->GetFName().ToString();
	if (playerChar) {
		playerChar->SetTarget(1);
		playerChar->targetString = hitObjectName;
	}
}

void AEnemy::MouseOverEnd() 
{
	playerChar = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (playerChar) 
	{
		playerChar->SetTarget(0);
		playerChar->targetString = "";
	}
}

// deal damage to drone
void AEnemy::Damage(int dmg) {
	if (isFriendly) return;
	if (vincible) return;

	if (enemyType == EnemyType::EnemyBase) {
		health -= dmg;
	}
	else if (enemyType == EnemyType::EnemyElite)
	{
		health -= dmg;
	}
	else if (enemyType == EnemyType::EnemyBoss)
	{
		if (vincible) health -= dmg;
	}
	else
	{
		health -= dmg;
	}

	// play dmg effect, sound & spawn broken actor
	PlaySound(0);
	
}

// 0: been hit; 1: attack; 
void AEnemy::PlaySound(int sound) 
{
	float volumeMultiplier = 1.f;
	float pitchMultiplier = 1.f;
	float startTime = 0.f;

	if (enemyType == EnemyType::EnemyBase || enemyType == EnemyType::EnemyElite || enemyType == EnemyType::EnemyBossSpawn)
	{
		UObject* worldContextObject = GetWorld();
		if (sound == 0)
		{
			int rnd = FMath::RandRange(0, 3);
			if (rnd == 1)
				UGameplayStatics::PlaySound2D(worldContextObject, soundHit, volumeMultiplier *.85f, pitchMultiplier * .9f, startTime);
			else if (rnd == 2)
				UGameplayStatics::PlaySound2D(worldContextObject, soundHit, volumeMultiplier *.8f, pitchMultiplier, startTime);
			else if (rnd == 3)
				UGameplayStatics::PlaySound2D(worldContextObject, soundHit, volumeMultiplier *.75f, pitchMultiplier * 1.2f, startTime);
		}
		else if (sound == 1)
		{
			int rnd = FMath::RandRange(0, 3);
			if (rnd == 1)
				UGameplayStatics::PlaySound2D(worldContextObject, soundSkill1a, volumeMultiplier, pitchMultiplier, startTime);
			else if (rnd == 2)
				UGameplayStatics::PlaySound2D(worldContextObject, soundSkill1b, volumeMultiplier, pitchMultiplier, startTime);
			else if (rnd == 3)
				UGameplayStatics::PlaySound2D(worldContextObject, soundSkill1c, volumeMultiplier, pitchMultiplier, startTime);
		}
	}
	else if (enemyType == EnemyType::EnemyBoss)
	{
		UObject* worldContextObject = GetWorld();
		if (sound == 0)
		{
			int rnd = FMath::RandRange(0, 3);
			if (rnd == 1)
				UGameplayStatics::PlaySound2D(worldContextObject, bossSoundHit, volumeMultiplier *.85f, pitchMultiplier * .9f, startTime);
			else if (rnd == 2)
				UGameplayStatics::PlaySound2D(worldContextObject, bossSoundHit, volumeMultiplier *.8f, pitchMultiplier, startTime);
			else if (rnd == 3)
				UGameplayStatics::PlaySound2D(worldContextObject, bossSoundHit, volumeMultiplier *.75f, pitchMultiplier * 1.2f, startTime);
		}
		else if (sound == 1)
		{
			int rnd = FMath::RandRange(0, 3);
			if (rnd == 1)
				UGameplayStatics::PlaySound2D(worldContextObject, bossSoundSkill1a, volumeMultiplier, pitchMultiplier, startTime);
			else if (rnd == 2)
				UGameplayStatics::PlaySound2D(worldContextObject, bossSoundSkill1b, volumeMultiplier, pitchMultiplier, startTime);
			else if (rnd == 3)
				UGameplayStatics::PlaySound2D(worldContextObject, bossSoundSkill1c, volumeMultiplier, pitchMultiplier, startTime);
		}
	}
	
		
}


void AEnemy::FollowActor(AActor* actor, float deltaTime)
{
	//auto lookAt = (actor->GetActorLocation() - this->GetActorLocation());
	//if (lookAt.Normalize())
	//{
	//	auto rotation = lookAt.Rotation();
	//	auto trafo = GetActorTransform();
	//}
	//// spawns default controller (if not, there is no controller for NPC to control)
	//this->SpawnDefaultController();
	//// cast controller recieved in NCP in Bar Class, derived from AAIController class
	//ABarNPCAIController * moveController = Cast<ABarNPCAIController>(NPC->GetController());
	//// moves character to location defined by TargetLocation component of moveEvent	
	//moveController->MoveToLocation(*moveEvent.targetLocation);

	/*float speed = 250.0f;

	auto lookAt = (actor->GetActorLocation() - this->GetActorLocation());
	if (lookAt.Normalize())
	{
		auto rotation = lookAt.Rotation();
		auto trafo = GetActorTransform();

		this->SetActorRotation(rotation);
		this->SetActorLocation(GetActorLocation() + (GetActorForwardVector() * speed * deltaTime));
	}*/
}

float AEnemy::GetFollowConstraint_Implementation()
{
	return 0.f;
}
float AEnemy::GetCoverConstraint_Implementation()
{
	return 0.f;
}
float AEnemy::GetRoamConstraint_Implementation()
{
	return 0.f;
}
float AEnemy::GetHealConstraint_Implementation()
{
	return 0.f;
}
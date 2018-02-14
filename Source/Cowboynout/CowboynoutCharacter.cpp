// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "CowboynoutCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Enemy.h"
#include "BossComponent.h"
#include "Blueprint/UserWidget.h"
#include "CowboynoutPlayerController.h"
#include "CowboynoutGameState.h"


ACowboynoutCharacter::ACowboynoutCharacter() {
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	muzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	muzzleLocation->SetupAttachment(RootComponent);

	muzzleLocationL = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocationL"));
	muzzleLocationL->SetupAttachment(RootComponent);

	muzzleLocationR = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocationR"));
	muzzleLocationR->SetupAttachment(RootComponent);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->MaxWalkSpeed = 600;  // increases speed by 10% per skill lvl

	// Create a camera boom... ermmm.... allready exists... kinda does nothing
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1000.f;
	CameraBoom->RelativeRotation = FRotator(0.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	// set cam range var for referencing
	camRange = CameraBoom->TargetArmLength;

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// ## var init
	targetString = "";
	dead = false;
	hasTarget = 0;
	deathTimer = 2.4f;
	deathTimerFull = deathTimer;
	hasTarget = 0;
	life = 100;
	lifeMax = 100;
	speed = 1;
	attack = 1;
	skillLvlOne = 1;
	skillLvlTwo = 1;
	chipsA = 0;
	chipsB = 0;
	chipsC = 0;
	medPacks = 0;
	nades = 2;

	// base level up gains in percent
	lifeGainPerLevel = 10;
	speedGainPerLevel = 10;
	attackGainPerLevel = 10;

	explodeNade = false;
	
	enemiesTotal = 1;
	bossBarrikades = 0;
	enemiesSet = false;
	barriersDisabled = 0;
	enemiesToDisableBarrier = 0;

	deathScreenSet = false;

	lifeWarningTimer = 3.11f;
	lifeWarningTimerFull = 3.11f;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	//CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ACowboynoutCharacter::OnOverlapBegin);

	soundBeenHit = LoadObject<USoundBase>(NULL, TEXT("SoundCue'/Game/Assets/Audio/Player/PlayerHit_Cue.PlayerHit_Cue'"), NULL, LOAD_None, NULL);
	dashSound = LoadObject<USoundBase>(NULL, TEXT("SoundCue'/Game/Assets/Audio/Player/Dash.Dash'"), NULL, LOAD_None, NULL);
	soundSkill1_Laser = LoadObject<USoundBase>(NULL, TEXT("SoundCue'/Game/Assets/Audio/Player/Skill_1_Cue.Skill_1_Cue'"), NULL, LOAD_None, NULL);
	soundSkill2_Nade = LoadObject<USoundBase>(NULL, TEXT("SoundCue'/Game/Assets/Audio/Player/AoeAttack_Cue.AoeAttack_Cue'"), NULL, LOAD_None, NULL);
	soundLowLife = LoadObject<USoundBase>(NULL, TEXT("SoundWave'/Game/Assets/Audio/Player/PlayerLowLife.PlayerLowLife'"), NULL, LOAD_None, NULL);
	soundDead = LoadObject<USoundBase>(NULL, TEXT("SoundWave'/Game/Assets/Audio/Player/PlayerDeath.PlayerDeath'"), NULL, LOAD_None, NULL);

	//ProjectileClassT1 = *LoadObject<TSubclassOf<AProjectile>>(NULL, TEXT("Blueprint'/Game/Blueprints/Skills/BP_SkillOne_T01.BP_SkillOne_T01'"), NULL, LOAD_None, NULL);
	//ProjectileClassT2 = *LoadObject<TSubclassOf<AProjectile>>(NULL, TEXT("Blueprint'/Game/Blueprints/Skills/BP_SkillOne_T02.BP_SkillOne_T02'"), NULL, LOAD_None, NULL);
	//ProjectileClassT3 = *LoadObject<TSubclassOf<AProjectile>>(NULL, TEXT("Blueprint'/Game/Blueprints/Skills/BP_SkillOne_T03.BP_SkillOne_T03'"), NULL, LOAD_None, NULL);

	//GrenadeClassT1 = LoadObject<TSubclassOf<AGrenade>>(NULL, TEXT("Blueprint'/Game/Blueprints/Skills/BP_SkillTwo_T01.BP_SkillTwo_T01'"), NULL, LOAD_None, NULL);
	//GrenadeClassT2 = LoadObject<TSubclassOf<AGrenade>>(NULL, TEXT("Blueprint'/Game/Blueprints/Skills/BP_SkillTwo_T02.BP_SkillTwo_T02'"), NULL, LOAD_None, NULL);
	//GrenadeClassT3 = LoadObject<TSubclassOf<AGrenade>>(NULL, TEXT("Blueprint'/Game/Blueprints/Skills/BP_SkillTwo_T03.BP_SkillTwo_T03'"), NULL, LOAD_None, NULL);
}


// set if the player has a target
void ACowboynoutCharacter::SetTarget(int targetStatus) {			/// 0: no target; 1: enemy target; 2: usable
	if (targetStatus == 0) {
		hasTarget = 0;
	}
	else if (targetStatus == 1) {
		hasTarget = 1;
	}
	else if (targetStatus == 2) {
		hasTarget = 2;
	}
	else hasTarget = 0;
}

void ACowboynoutCharacter::Tick(float DeltaSeconds) {
    Super::Tick(DeltaSeconds);

	CameraBoom->TargetArmLength = camRange;
	//CameraBoom->SetWorldRotation = camAngle;

#pragma region get enemy ammount

	if (enemiesActual>enemiesTotal) {
		// set total number of enemies
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), foundActors);
		enemiesTotal = foundActors.Num() -1;
	}
	
	// set actual number of enemies
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), foundActors);
	for (int i = 0; i < foundActors.Num(); i++) {
		AEnemy* enemiski = Cast<AEnemy>(foundActors[i]);
		if (enemiski->isBoss)
		{
			enemiesActual = enemiski->bossDrones.Num();
			//enemiesTotal = enemiski->bossDronesToSpawnThisPhase;
			
			TArray<UBossComponent*> Components;
			enemiski->GetComponents<UBossComponent>(Components);
			if (Components[0]->IsValidLowLevel()) enemiesTotal = Components[0]->stages[Components[0]->stateSwitchesCount].dronesToSpawn;
		}
	}

	if (enemiesTotal != 0 && bossBarrikades != 0)
		enemiesToDisableBarrier = enemiesTotal / bossBarrikades;
#pragma endregion 

#pragma region set game state (paused/playing)
	ACowboynoutGameState* state = Cast<ACowboynoutGameState>(GetWorld()->GetGameState());
	if (state)
	{
		if (!state->isPaused)
		{
		}
	}
	if (CursorToWorld != nullptr)
	{
		APlayerController* playerCtrl = Cast<APlayerController>(GetController());
		if (playerCtrl)
		{
			FHitResult TraceHitResult;
			playerCtrl->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
#pragma endregion 

#pragma region Life stuff
	// play low life sound warning
	if (life <= lifeWarningValue) {
		if (lifeWarningTimer > lifeWarningTimerFull) {
			PlaySound(4);
			lifeWarningTimer = 0;
		}
		else {
			lifeWarningTimer += DeltaSeconds;
		}
	}

	if (dead) {
		if (deathTimer > 0) {
			deathTimer -= DeltaSeconds;
			//FString msg = FString::SanitizeFloat(deathTimerFull - (deathTimerFull - deathTimer));
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, msg);
		}
		if (deathTimer <= 0 && !deathScreenActive)
		{
			deathScreenActive = true;
			ACowboynoutPlayerController* playerCtrl = Cast<ACowboynoutPlayerController>(GetController());
			playerCtrl->ToggleDeathScreen();
			//UGameplayStatics::OpenLevel(this, TEXT("/Game/Maps/DeathScreen_Menu"), false);
		}
	}
#pragma endregion 

#pragma region auto stat upgrades
	// check for chip update
	if (chipsA > 5) {
		ConvertChipStatA(5);
	}
	if (chipsB > 5) {
		ConvertChipStatB(5);
	}
	if (chipsC > 5) {
		ConvertChipStatC(5);
	}
#pragma endregion 
}

void ACowboynoutCharacter::Damage(int dmg) {
	ACowboynoutPlayerController* playerCtrl = Cast<ACowboynoutPlayerController>(GetController());
	if (playerCtrl) 
		if (playerCtrl->isDashing) return; 
	
		else
		{
			life -= dmg;

			PlaySound(0);
			if (life <= 0) {
				PlaySound(5);
				Die();
				animDead = true;
			}
		}
	
}

float ACowboynoutCharacter::GetHealth() {
	float healthReturn = life / lifeMax;
	return healthReturn;
}

void ACowboynoutCharacter::Die() {
	dead = true;
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "ouch. dead man walking!");
	animDying = true;
}

// converts [ammount of chip A] of chips to upgrade stat A : life, blue
void ACowboynoutCharacter::ConvertChipStatA(int ammount) {
	if (chipsA >= ammount) {
		chipsA -= ammount;
		lifeMax += lifeGainPerLevel;
		//life = lifeMax;
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "not enough chips to convert to stat A");
}

// converts [ammount of chip B] of chips to upgrade stat B : speed, green
void ACowboynoutCharacter::ConvertChipStatB(int ammount) {
	if (chipsB >= ammount) {
		chipsB -= ammount;
		speed++;
		GetCharacterMovement()->MaxWalkSpeed += speedGainPerLevel;
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "not enough chips to convert to stat B");
}

// converts [ammount of chip C] of chips to upgrade stat C : attack, red
void ACowboynoutCharacter::ConvertChipStatC(int ammount) {
	if (chipsC >= ammount) {
		chipsC -= ammount;
		attack += 1;
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "not enough chips to convert to stat C");
}

// converts [ammount of chip A and B] of chips to upgrade skill A
void ACowboynoutCharacter::ConvertChipSkillA(int ammount) {
	if (chipsA >= ammount && chipsB >= ammount) {
		chipsA -= ammount;
		chipsB -= ammount;
		skillLvlOne++;
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "not enough chips to convert to skill A");
}

// converts [ammount of chip B and C] of chips to upgrade skill B
void ACowboynoutCharacter::ConvertChipSkillB(int ammount) {
	if (chipsB >= ammount && chipsC >= ammount) {
		chipsB -= ammount;
		chipsC -= ammount;
		skillLvlTwo++;
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "not enough chips to convert to skill B");
}

// returns ammount of chips Stat A : life
int ACowboynoutCharacter::GetChipsA() {
	return chipsA;
}

// returns ammount of chips Stat B : speed
int ACowboynoutCharacter::GetChipsB() {
	return chipsB;
}

// returns ammount of chips Stat C : attack
int ACowboynoutCharacter::GetChipsC() {
	return chipsC;
}

// returns ammount of grenades
int ACowboynoutCharacter::GetNades() {
	return nades;
}

void ACowboynoutCharacter::FireSkillOne() {
	ACowboynoutGameState* gs = Cast<ACowboynoutGameState>(GetWorld()->GetGameState());
	if (dead || gs->isPaused) return;
	ACowboynoutPlayerController* playerCtrl = Cast<ACowboynoutPlayerController>(GetController());
	if (playerCtrl) {
		animShooting = true;

		FActorSpawnParameters spawnInfo;
		if (skillLvlOne == 1) {
			AProjectile* bullet = GetWorld()->SpawnActor<AProjectile>(ProjectileClassT1, muzzleLocation->GetComponentLocation(), muzzleLocation->GetComponentRotation(), spawnInfo);
			if (bullet) bullet->bulletType = BulletType::PlayerBullet;
		}
		else if (skillLvlOne == 2) {
			AProjectile* bullet = GetWorld()->SpawnActor<AProjectile>(ProjectileClassT2, muzzleLocation->GetComponentLocation(), muzzleLocation->GetComponentRotation(), spawnInfo);
			if (bullet) bullet->bulletType = BulletType::PlayerBullet;
		}
		else if (skillLvlOne == 3) {
			// multiple projectiles spawned
			FRotator rotL = muzzleLocation->GetComponentRotation();
			rotL.Yaw += 30;
			FRotator rotR = muzzleLocation->GetComponentRotation();
			rotR.Yaw -= 30;
			AProjectile* bulletL = GetWorld()->SpawnActor<AProjectile>(ProjectileClassT1, muzzleLocationL->GetComponentLocation(), rotL, spawnInfo);
			if (bulletL) bulletL->bulletType = BulletType::PlayerBullet;
			AProjectile* bullet = GetWorld()->SpawnActor<AProjectile>(ProjectileClassT3, muzzleLocation->GetComponentLocation(), muzzleLocation->GetComponentRotation(), spawnInfo);
			if (bullet)  bullet->bulletType = BulletType::PlayerBullet;
			AProjectile* bulletR = GetWorld()->SpawnActor<AProjectile>(ProjectileClassT1, muzzleLocationR->GetComponentLocation(), rotR, spawnInfo);
			if (bulletR) bulletR->bulletType = BulletType::PlayerBullet;
		}
		PlaySound(1);
	}
}

void ACowboynoutCharacter::FireSkillTwo() {
	ACowboynoutGameState* gs = Cast<ACowboynoutGameState>(GetWorld()->GetGameState());
	if (dead || gs->isPaused) return;

	nadeLoc = GetActorLocation();
	animShooting = true;
	explodeNade = false;
		
	ACowboynoutPlayerController* playerCtrl = Cast<ACowboynoutPlayerController>(GetController());
	if (playerCtrl) {
		if (nades > 0) {
			FActorSpawnParameters spawnInfo;
			spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			FRotator rot;
			AGrenade* nade = GetWorld()->SpawnActor<AGrenade>(GrenadeClassT1, GetActorLocation(), rot, spawnInfo);
				
			PlaySound(2);
			nades--;
		}
	}
}

// (0) been hit; (1) laser; (2) nade; (3) dash; (4) low life warning; (5) death;
void ACowboynoutCharacter::PlaySound(int sound) {
	float volumeMultiplier = 1.f;
	float pitchMultiplier = 1.f;
	float startTime = 0.f;

	UObject* worldContextObject = GetWorld();
	
	if (sound == 0)
		UGameplayStatics::PlaySound2D(worldContextObject, soundBeenHit, volumeMultiplier, pitchMultiplier, startTime);
	else if (sound == 1)
		UGameplayStatics::PlaySound2D(worldContextObject, soundSkill1_Laser, volumeMultiplier, pitchMultiplier, startTime);
	else if (sound == 2)
		UGameplayStatics::PlaySound2D(worldContextObject, soundSkill2_Nade, volumeMultiplier, pitchMultiplier, startTime);
	else if (sound == 3)
		UGameplayStatics::PlaySound2D(worldContextObject, dashSound, volumeMultiplier, pitchMultiplier, startTime);
	else if (sound == 4)
		UGameplayStatics::PlaySound2D(worldContextObject, soundLowLife, volumeMultiplier, pitchMultiplier, startTime);
	else if (sound == 5)
		UGameplayStatics::PlaySound2D(worldContextObject, soundDead, volumeMultiplier, pitchMultiplier, startTime);

}

void ACowboynoutCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "FU!");
	// AE DMG 
	if (OtherComp != NULL && OtherComp->ComponentHasTag("dmgArea")) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "AE DOMAGE!");
		Damage(10);
	}
}
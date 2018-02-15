// You no take candle!

#include "CamTriggerVolume.h"
// include draw debug helpers header file
#include "DrawDebugHelpers.h"
#include "CowboynoutCharacter.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(text), fstring))

ACamTriggerVolume::ACamTriggerVolume()
{
	//Register Events
	OnActorBeginOverlap.AddDynamic(this, &ACamTriggerVolume::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ACamTriggerVolume::OnOverlapEnd);
}

void ACamTriggerVolume::BeginPlay()
{
	Super::BeginPlay();

	//DrawDebugBox(GetWorld(), GetActorLocation(), GetActorScale() * 100, FColor::Cyan, true, -1, 0, 5);
	cowboy = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (camRangeToSet == 0) camRangeToSet = 1000.f;
	if (camAngleToSet == 0) camAngleToSet = 70.f;
}

void ACamTriggerVolume::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this)) {
		// print to screen using above defined method when actor enters trigger volume
		//print("Overlap Begin");
		//printFString("Other Actor = %s", *OtherActor->GetName());

		if (OtherActor->GetName() == "BP_Character_C_0")
		{
			//print("Char in");
			if (!cowboy) cowboy = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (cowboy) 
			{
				savedCamAngle = cowboy->camAngle;
				savedCamRange = cowboy->camRange;
				cowboy->camRange = camRangeToSet;
			}
		}
			
	}
}

void ACamTriggerVolume::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this)) {
		// print to screen using above defined method when actor leaves trigger volume
		//print("Overlap Ended");
		//printFString("%s has left the Trigger Volume", *OtherActor->GetName());

		if (OtherActor->GetName() == "BP_Character_C_0")
		{
			//print("Char out");
			if (!cowboy) cowboy = Cast<ACowboynoutCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (cowboy)
			{
				cowboy->camRange = savedCamRange;
				cowboy->camAngle = savedCamAngle;
			}
		}
			
	}
}
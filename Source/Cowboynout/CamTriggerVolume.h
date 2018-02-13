// You no take candle!

#pragma once

#include "Engine.h"
#include "Engine/TriggerVolume.h"
#include "CowboynoutCharacter.h"
#include "CamTriggerVolume.generated.h"

/**
 * 
 */
UCLASS()
class COWBOYNOUT_API ACamTriggerVolume : public ATriggerVolume
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// constructor sets default values for this actor's properties
	ACamTriggerVolume();

	// overlap begin function
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	// overlap end function
	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
	float camAngleToSet;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
	float camRangeToSet;

	UPROPERTY()
	ACowboynoutCharacter* cowboy;
	
	UPROPERTY()
	float savedCamAngle;

	UPROPERTY()
	float savedCamRange;
};

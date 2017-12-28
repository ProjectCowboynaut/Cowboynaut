// you no take candle!

#pragma once

#include "Array.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Curves/CurveFloat.h"
#include "AAIAgent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionDelegate);

USTRUCT()
struct FEntry
{
	GENERATED_BODY()
	
	UPROPERTY() UCurveFloat* curve;
	FActionDelegate action;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COWBOYNOUT_API UAAIAgent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAAIAgent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//UFUNCTION()
	//void AddAction(UCurveFloat* curve, FActionDelegate action);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	TArray<FEntry *> entries;

	float currentNeed;
	FActionDelegate currentAction;

};

// you no take candle!

#pragma once

#include "Array.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Curves/CurveFloat.h"
#include "AAIAgent.generated.h"

UENUM(Blueprintable)
enum class FActionType : uint8
{
	FATFollowPlayer UMETA(DisplayName = "Follow Player"),
	FATTakeCover UMETA(DisplayName = "Take Cover"),
	FATRoaming UMETA(DisplayName = "Roam Around")
};

USTRUCT(Blueprintable)
struct FAction
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "AI") FActionType actionType;
	UPROPERTY(EditAnywhere, Category = "AI") UCurveFloat* considerationCurve;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COWBOYNOUT_API UAAIAgent : public UActorComponent
{
	GENERATED_BODY()

public:

	UAAIAgent();

	UFUNCTION(BlueprintCallable, Category = "AI") void AddAction(const FAction& action);
	UFUNCTION(BlueprintCallable, Category = "AI") FActionType EvaluateActions();
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI") TArray actionList;

protected:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};

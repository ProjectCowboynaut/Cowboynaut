// you no take candle!

#pragma once

#include "Array.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Curves/CurveFloat.h"
#include "AAIAgent.generated.h"

DECLARE_DYNAMIC_DELEGATE_RetVal(float, FActionDelegate);

UENUM(Blueprintable)
enum class FActionType : uint8
{
	FATFollowPlayer UMETA(DisplayName = "Follow Player"),
	FATTakeCover UMETA(DisplayName = "Take Cover"),
	FATRoaming UMETA(DisplayName = "Roam Around"),
	FATNeedHeal UMETA(DisplayName = "Need to heal")
};

USTRUCT(Blueprintable)
struct FAction
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI") FActionType actionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI") UCurveFloat* considerationCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI") FActionDelegate actionAxisConstraint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI") float actionLockTime;

};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COWBOYNOUT_API UAAIAgent : public UActorComponent
{
	GENERATED_BODY()

public:

	UAAIAgent();

	UFUNCTION(BlueprintCallable, Category = "AI") void AddAction(const FAction& action);

	UFUNCTION(BlueprintCallable, Category = "AI") FActionType EvaluateActions();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI") TArray<FAction> actionList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI") FAction currentAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI") float currentLockTime;

protected:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};

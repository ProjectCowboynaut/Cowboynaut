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
	FATRoaming UMETA(DisplayName = "Roam Around"),
	FATNeedHeal UMETA(DisplayName = "Need to heal")
};

USTRUCT(Blueprintable)
struct FEvaluationInput
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input") float currentHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input") float maxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input") float rangeToHealer;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input") float rangeToPlayer;

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

	UFUNCTION(BlueprintCallable, Category = "AI") FActionType EvaluateActions(const FEvaluationInput& evaluationInput);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI") TArray<FAction> actionList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI") float perceptionRange;

protected:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};

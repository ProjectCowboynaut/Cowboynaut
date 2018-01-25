#include "AAIAgent.h"
#include "Cowboynout.h"

UAAIAgent::UAAIAgent() :
	perceptionRange(3000.f)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAAIAgent::AddAction(const FAction& action)
{
	this->actionList.Add(action);
}

FActionType UAAIAgent::EvaluateActions(const FEvaluationInput& evaluationInput)
{
	if (currentLockTime < currentAction.actionLockTime)
	{
		currentLockTime -= GetWorld()->GetDeltaSeconds();
		return this->currentAction.actionType;
	}

	float currentPriority = 0.0f;

	for (auto& action : this->actionList)
	{
		float priority = 0.0f;

		switch (action.actionType)
		{
		case FActionType::FATFollowPlayer:
			priority = action.considerationCurve->GetFloatValue(
				evaluationInput.rangeToPlayer / this->perceptionRange
			);
			break;
		case FActionType::FATRoaming:
			priority = action.considerationCurve->GetFloatValue(
				evaluationInput.rangeToPlayer / this->perceptionRange
			);
			break;
		case FActionType::FATTakeCover:
			priority = action.considerationCurve->GetFloatValue(
				evaluationInput.rangeToPlayer / this->perceptionRange
			);
			break;
		case FActionType::FATNeedHeal:
			priority = action.considerationCurve->GetFloatValue(
				evaluationInput.currentHealth / evaluationInput.maxHealth
			);
			break;
		}

		if (priority > currentPriority)
		{
			currentPriority = priority;
			currentAction = action;
			currentLockTime = currentAction.actionLockTime;
		}
	}

	return currentAction.actionType;
}


void UAAIAgent::BeginPlay()
{
	Super::BeginPlay();
}

void UAAIAgent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
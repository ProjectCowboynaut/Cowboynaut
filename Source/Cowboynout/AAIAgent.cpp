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

FActionType UAAIAgent::EvaluateActions()
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
		float inputValue = 0.0f;

		inputValue = action.actionAxisConstraint.Execute();
		priority = action.considerationCurve->GetFloatValue(inputValue);

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
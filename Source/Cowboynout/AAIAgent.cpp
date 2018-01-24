#include "AAIAgent.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red, text)

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
	FAction* currentAction = nullptr;
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
			currentAction = &action;
		}
	}

	if (!currentAction)
		return FActionType::FATRoaming;
	else
		return currentAction->actionType;
}


void UAAIAgent::BeginPlay()
{
	Super::BeginPlay();
}

void UAAIAgent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// MAYBE LIMIT THE TICK RATE FOR AI UPDATES (ELSE MAY EXPLODE)

	/*for (auto entry : entries)
	{
		// poll value from curve (appropriate input)
		auto potentialNeed = entry->curve->GetFloatValue(0);

		if (potentialNeed > currentNeed)
		{
			// there is a more urgent action; override!
			currentNeed = potentialNeed;
			currentAction = entry->action;
		}
	}

	// found most urgent action; process it!
	currentAction.Broadcast();*/
}
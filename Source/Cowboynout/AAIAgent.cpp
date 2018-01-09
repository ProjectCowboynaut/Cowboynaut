#include "AAIAgent.h"

UAAIAgent::UAAIAgent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAAIAgent::AddAction(const FAction& action)
{

}

FActionType UAAIAgent::EvaluateActions()
{
	return FActionType::FATFollowPlayer;
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
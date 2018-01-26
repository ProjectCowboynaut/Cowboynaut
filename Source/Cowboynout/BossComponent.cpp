// you no take candle!

#include "BossComponent.h"


UBossComponent::UBossComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UBossComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UBossComponent::BossFight() {

}

void UBossComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


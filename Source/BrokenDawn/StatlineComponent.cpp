#include "StatlineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UStatlineComponent::UStatlineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UStatlineComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UStatlineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TickType != ELevelTick::LEVELTICK_PauseTick)
	{
		Health.TickStat(DeltaTime);
		TickStamina(DeltaTime);
	}
}

float UStatlineComponent::GetStatPercentile(const ECoreStat Stat) const
{
	switch (Stat)
	{
	case ECoreStat::CS_HEALTH:	return Health.Percentile();
	case ECoreStat::CS_STAMINA: return Stamina.Percentile();
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid stat!"));
		break;
	}

	return -1;
}


float UStatlineComponent::GetStatValue(const ECoreStat Stat) const
{
	switch (Stat)
	{
	case ECoreStat::CS_HEALTH:  return Health.CurrentValue;
	case ECoreStat::CS_STAMINA: return Stamina.CurrentValue;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid stat!"));
		break;
	}

	return -1;
}

void UStatlineComponent::HasJumped(const bool bValue)
{
	bHasJumped = bValue;
}

void UStatlineComponent::IsSprinting(const bool bValue)
{
	bIsSprinting = bValue;
}

void UStatlineComponent::TickStamina(const float& DeltaTime)
{
	if (bHasJumped)
	{
		Stamina.TickStat(-JumpCost);
		return;
	}
	if (bIsSprinting)
	{
		Stamina.TickStat(-(DeltaTime * SprintCostMultiplier));
		return;
	}
	Stamina.TickStat(DeltaTime * StaminaRecoveryMultiplier);
}
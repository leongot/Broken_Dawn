#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BDCharacterMovementComponent.generated.h"


UCLASS()
class BROKENDAWN_API UBDCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	// Sets default values for this component's properties
	UBDCharacterMovementComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Sprint();
	void StopSprinting();

private:
	bool bIsSprinting;
};

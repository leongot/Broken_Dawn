#include "BDPlayerCameraManager.h"

#include "BDPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABDPlayerCameraManager::ABDPlayerCameraManager()
{

}

void ABDPlayerCameraManager::UpdateViewTarget(FTViewTarget& VT, float DeltaTime)
{
	Super::UpdateViewTarget(VT, DeltaTime);

	if (ABDPlayerCharacter* BDPlayerCharacter = Cast<ABDPlayerCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		UCharacterMovementComponent* CMC = BDPlayerCharacter->GetCharacterMovement();
		
		float CapsuleHeight = BDPlayerCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		FVector TargetCrouchOffset = FVector(0.0f, 0.0f, CMC->GetCrouchedHalfHeight() - CapsuleHeight);
		FVector Movement = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.0f, 1.0f));
		
		if (CMC->IsCrouching())	
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.0f, CrouchBlendDuration);
		else
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.0f, CrouchBlendDuration);

		VT.POV.Location += Movement;
	}
}

#pragma once

#include "CoreMinimal.h"
#include "BDPlayerCameraManager.generated.h"

UCLASS()
class BROKENDAWN_API ABDPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:

	ABDPlayerCameraManager();

	void UpdateViewTarget(FTViewTarget& VT, float DeltaTime) override;

private:

	UPROPERTY(EditDefaultsOnly)
	float CrouchBlendTime;

	float CrouchBlendDuration = 0.5f;
};

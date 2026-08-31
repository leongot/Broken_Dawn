#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatlineComponent.generated.h"

UENUM(BlueprintType)
enum class ECoreStat : uint8
{
	CS_HEALTH UMETA(DisplayName = "Health"),
	CS_STAMINA UMETA(DisplayName = "Stamina")
};

USTRUCT(BlueprintType)
struct FCoreStat
{
	GENERATED_USTRUCT_BODY()

	void TickStat(const float& DeltaTime)
	{
		CurrentValue = FMath::Clamp(CurrentValue + (PerSecondTick * DeltaTime), 0, MaxValue);
	}

	void Adjust(const float& Amount)
	{
		CurrentValue = FMath::Clamp(CurrentValue + Amount, 0, MaxValue);
	}

	float Percentile() const
	{
		return CurrentValue / MaxValue;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentValue = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxValue = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float PerSecondTick = 1.0f;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BROKENDAWN_API UStatlineComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatlineComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	float GetStatPercentile(const ECoreStat Stat) const;

	UFUNCTION(BlueprintCallable)
	float GetStatValue(const ECoreStat Stat) const;

	UFUNCTION(BlueprintCallable)
	void HasJumped(const bool bValue);

	UFUNCTION(BlueprintCallable)
	void IsSprinting(const bool bValue);

	UFUNCTION(BlueprintCallable)
	void TickStamina(const float& DeltaTime);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// --- Parametri Gestione Stamina ---

	/** Costo di stamina per ciascun salto */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Stamina")
	float JumpCost = 15.0f;

	/** Moltiplicatore del consumo di stamina al secondo durante lo scatto */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Stamina")
	float SprintCostMultiplier = 10.0f;

	/** Moltiplicatore di rigenerazione stamina al secondo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Stamina")
	float StaminaRecoveryMultiplier = 15.0f;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Stamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bHasJumped = false;
};
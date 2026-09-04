#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h" 
#include "InputActionValue.h"
#include "StatlineComponent.h"
#include "BDPlayerCharacter.generated.h"

class UAnimBlueprint;
class UInputMappingContext;
class UInputAction;
class UIInputComponent;
//class UStatlineComponent;
class UCharacterMovementComponent;


UCLASS()
class BROKENDAWN_API ABDPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABDPlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> CrouchAction;


private:

	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& InputActionValue);

	UFUNCTION(BlueprintCallable)
	void MouseLook(const FInputActionValue& InputActionValue);

	UFUNCTION(BlueprintCallable)
	void Sprint(const FInputActionValue& InputActionValue);

	UFUNCTION(BlueprintCallable)
	void StopSprinting();

	UFUNCTION(BlueprintCallable)
	void ToggleCrouch();

	void _Jump();
	void _StopJumping();

	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	//UStatlineComponent* Statline;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FVector CameraOffset = FVector(2.8f, 5.9f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Camera")
	float FieldOfView = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float FirstPersonScale = 0.8f;


	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed = 450.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintSpeed = 750.0f;


	UPROPERTY(EditAnywhere, Category = "Stats")
	FCoreStat Health;

	UPROPERTY(EditAnywhere, Category = "Stats")
	FCoreStat Stamina;

	UPROPERTY(EditAnywhere, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float SprintCostMultiplier = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float JumpCost = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float StaminaRecoveryMultiplier = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float StaminaRecoveryDelay = 0.5f;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> PlayerMeshComponent;

	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<UAnimBlueprint> FirstPersonAnim;
	
};

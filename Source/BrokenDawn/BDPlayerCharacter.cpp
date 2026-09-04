#include "BDPlayerCharacter.h"
#include "StatlineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABDPlayerCharacter::ABDPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	check(CameraComp);
 
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));
	check(SkeletalMeshComp);
	
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	
	SkeletalMeshComp->SetupAttachment(GetMesh());												
	SkeletalMeshComp->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;			
	SkeletalMeshComp->SetCollisionProfileName(FName("NoCollision"));
	
	CameraComp->SetupAttachment(SkeletalMeshComp, FName("head"));
	
	CameraComp->bUsePawnControlRotation = true;
	
	CameraComp->bEnableFirstPersonFieldOfView = true;
	CameraComp->bEnableFirstPersonScale       = true;
	CameraComp->FirstPersonFieldOfView        = FieldOfView;
	CameraComp->FirstPersonScale		      = FirstPersonScale;

	CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->MaxWalkSpeed = MaxWalkSpeed;
	CharacterMovementComponent->GravityScale = GravityScale;
	CharacterMovementComponent->MaxAcceleration = MaxAcceleration;
	CharacterMovementComponent->GroundFriction = GroundFriction;
	CharacterMovementComponent->BrakingDecelerationWalking = BreakingDeceleration;
	CharacterMovementComponent->JumpZVelocity = JumpZVelocity;
	CharacterMovementComponent->AirControl = AirControl;
	CharacterMovementComponent->PerchRadiusThreshold = PerchRadiusThreshold;
}


void ABDPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Done via blueprint 
	//PlayerMeshComponent->SetAnimInstanceClass(FirstPersonAnim->GeneratedClass);
	//GetMesh()->SetAnimInstanceClass(FirswtPersonAnim->GeneratedClass);
	
	SkeletalMeshComp->SetOwnerNoSee(false);
	SkeletalMeshComp->CastShadow = true;
	SkeletalMeshComp->bCastHiddenShadow = true;

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void ABDPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABDPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Checks if EnhancedInputComponent exits
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction,   ETriggerEvent::Triggered, this, &ABDPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(JumpAction,   ETriggerEvent::Started,   this, &ABDPlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction,   ETriggerEvent::Completed, this, &ABDPlayerCharacter::StopJumping);
		EnhancedInputComponent->BindAction(LookAction,   ETriggerEvent::Triggered, this, &ABDPlayerCharacter::MouseLook);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started,   this, &ABDPlayerCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABDPlayerCharacter::StopSprinting);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ABDPlayerCharacter::ToggleCrouch);
	}
}

void ABDPlayerCharacter::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementValue = InputActionValue.Get<FVector2D>();

	constexpr float Multiplier = 0.5f;

	const FVector RightVec = GetActorRightVector();
	AddMovementInput(RightVec, MovementValue.X * Multiplier);

	const FVector ForwardVec = GetActorForwardVector();	
	AddMovementInput(ForwardVec, MovementValue.Y);
}

void ABDPlayerCharacter::MouseLook(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisValue = InputActionValue.Get<FVector2D>();

	AddControllerYawInput(LookAxisValue.X);
	AddControllerPitchInput(LookAxisValue.Y);
}

void ABDPlayerCharacter::Sprint(const FInputActionValue& InputActionValue)
{
	bool bCanSprint = CharacterMovementComponent->Velocity.Length() < SprintSpeed
				   && !CharacterMovementComponent->IsFalling();
	CharacterMovementComponent->MaxWalkSpeed = bCanSprint ? SprintSpeed : MaxWalkSpeed;
}

void ABDPlayerCharacter::StopSprinting()
{
	CharacterMovementComponent->MaxWalkSpeed = MaxWalkSpeed;
}

void ABDPlayerCharacter::ToggleCrouch()
{
	if (bIsCrouched) 
		UnCrouch();
	else 
		Crouch();
}

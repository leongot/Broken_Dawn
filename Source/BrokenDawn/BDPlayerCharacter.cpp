#include "BDPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABDPlayerCharacter::ABDPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
 
	PlayerMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));
	
	PlayerMeshComponent->SetupAttachment(GetMesh());												// Attaches the player mesh to the skeletal mesh
	PlayerMeshComponent->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::None;			// Includes the first person mesh in First-Person rendering
	PlayerMeshComponent->SetCollisionProfileName(FName("NoCollision"));								// Sets the first person mesh not to collide with other objects

	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;		// First-PErson rendering will treat the third-person mesh as regular geometry
	
	Camera->SetupAttachment(PlayerMeshComponent, FName("head"));							// Attaches the camera to player skeletal mesh
	
	Camera->bUsePawnControlRotation = true;
	
	Camera->bEnableFirstPersonFieldOfView = true;
	Camera->bEnableFirstPersonScale       = true;
	Camera->FirstPersonFieldOfView        = FieldOfView;
	Camera->FirstPersonScale		      = FirstPersonScale;
	
	CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->MaxWalkSpeed = WalkSpeed;
}

// Called when the game starts or when spawned
void ABDPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);

	PlayerMeshComponent->SetAnimInstanceClass(FirstPersonAnim->GeneratedClass);
	GetMesh()->SetAnimInstanceClass(FirstPersonAnim->GeneratedClass);

	PlayerMeshComponent->SetOnlyOwnerSee(true);
	PlayerMeshComponent->CastShadow = true;          // Forza il cast dell'ombra
	PlayerMeshComponent->bCastHiddenShadow = true;  // Permette di proiettare l'ombra anche se nascosto con SetOnlyOwnerSee

	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->CastShadow = true;
	GetMesh()->bCastHiddenShadow = true;

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

// Called every frame
void ABDPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
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

	constexpr  float Multiplier = 0.5f;

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
	CharacterMovementComponent->MaxWalkSpeed = bCanSprint ? SprintSpeed : WalkSpeed;
}

void ABDPlayerCharacter::StopSprinting()
{
	CharacterMovementComponent->MaxWalkSpeed = WalkSpeed;
}

void ABDPlayerCharacter::ToggleCrouch()
{
	if (bIsCrouched) 
		UnCrouch();
	else 
		Crouch();
}

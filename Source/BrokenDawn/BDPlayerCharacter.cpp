#include "BDPlayerCharacter.h"
#include "StatlineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABDPlayerCharacter::ABDPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!Controller) 
		UE_LOG(LogTemp, Error, TEXT("[BDPlayerCharacter] Controller error!"));

	 

	// Component creation
	//-------------------
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	check(Camera != nullptr);
 
	PlayerMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));
	check(PlayerMeshComponent != nullptr);
	//-------------------


	// First person setup
	//-------------------
	PlayerMeshComponent->SetupAttachment(GetMesh());												// Attaches the player mesh to the skeletal mesh
	PlayerMeshComponent->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::None;			// Includes the first person mesh in First-Person rendering
	PlayerMeshComponent->SetCollisionProfileName(FName("NoCollision"));								// Sets the first person mesh not to collide with other objects
	//-------------------

	// Third person mesh setup
	//-------------------
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;		// First-PErson rendering will treat the third-person mesh as regular geometry

	
	// Camera setup
	//-------------------
	Camera->SetupAttachment(PlayerMeshComponent, FName("head"));							// Attaches the camera to player skeletal mesh

	Camera->SetRelativeLocationAndRotation(CameraOffset, FRotator(0.0f, 90.0f, -90.0f));	
	Camera->bUsePawnControlRotation = true;
	
	Camera->bEnableFirstPersonFieldOfView = true;
	Camera->bEnableFirstPersonScale       = true;
	Camera->FirstPersonFieldOfView        = FieldOfView;
	Camera->FirstPersonScale		      = FirstPersonScale;
	//-------------------

	//Statline = CreateDefaultSubobject<UStatlineComponent>(TEXT("Statline"));
	//Statline->CharacterMovementComponent = GetCharacterMovement();

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

	// Test log
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Using BDPlayerCharacter."));
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
		EnhancedInputComponent->BindAction(JumpAction,   ETriggerEvent::Started,   this, &ABDPlayerCharacter::_Jump);
		EnhancedInputComponent->BindAction(JumpAction,   ETriggerEvent::Completed, this, &ABDPlayerCharacter::_StopJumping);
		EnhancedInputComponent->BindAction(LookAction,   ETriggerEvent::Triggered, this, &ABDPlayerCharacter::MouseLook);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started,   this, &ABDPlayerCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABDPlayerCharacter::StopSprinting);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ABDPlayerCharacter::ToggleCrouch);
	}
}

void ABDPlayerCharacter::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementValue = InputActionValue.Get<FVector2D>();

	const float Multiplier = 0.5f;

	const FVector RightVec = GetActorRightVector();
	AddMovementInput(RightVec, MovementValue.X * Multiplier);

	const FVector ForwardVec = GetActorForwardVector();
	if (MovementValue.Y <= 0.0f)
		AddMovementInput(ForwardVec, MovementValue.Y * Multiplier);
	else
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
	//if (Statline->GetStatValue(ECoreStat::CS_STAMINA) <= 0.0f) return;

	bool bCanSprint = CharacterMovementComponent->Velocity.Length() < SprintSpeed
				   && !CharacterMovementComponent->IsFalling();
	CharacterMovementComponent->MaxWalkSpeed = bCanSprint ? SprintSpeed : WalkSpeed;

	//Statline->IsSprinting(bCanSprint);

	if (bCanSprint)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("SPRINTING"));
}

void ABDPlayerCharacter::StopSprinting()
{
	CharacterMovementComponent->MaxWalkSpeed = WalkSpeed;
	//Statline->IsSprinting(false);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("STOP SPRINTING"));	
}

void ABDPlayerCharacter::ToggleCrouch()
{
	if (bIsCrouched) 
		UnCrouch();
	else 
		Crouch();
}

void ABDPlayerCharacter::_Jump()
{
	Jump();
	//Statline->HasJumped(true);
}

void ABDPlayerCharacter::_StopJumping()
{
	StopJumping();
	//Statline->HasJumped(false);
}
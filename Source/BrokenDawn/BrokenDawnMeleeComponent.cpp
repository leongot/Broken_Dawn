#include "BrokenDawnMeleeComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"

UBrokenDawnMeleeComponent::UBrokenDawnMeleeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bRandomizeAttacks = false;
	bEnableFriendlyFire = false;
	ComboIndex = 0;
}

void UBrokenDawnMeleeComponent::BeginPlay()
{
	Super::BeginPlay();
}

float UBrokenDawnMeleeComponent::PerformLightAttack()
{
	if (bIsAttacking)
	{
		if (bCanCombo)
		{
			bSavedAttack = true;
		}
		return 0.0f;
	}

	if (LightAttackMontages.Num() == 0) return 0.0f;

	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	if (!CharacterOwner) return 0.0f;

	TArray<USkeletalMeshComponent*> MeshComponents;
	CharacterOwner->GetComponents<USkeletalMeshComponent>(MeshComponents);
	if (MeshComponents.Num() == 0) return 0.0f;

	int32 SelectedIndex = bRandomizeAttacks ? FMath::RandRange(0, LightAttackMontages.Num() - 1) : ComboIndex;
	UAnimMontage* CurrentMontage = LightAttackMontages[SelectedIndex];
	if (!CurrentMontage) return 0.0f;

	bIsAttacking = true;
	bCanCombo = false;
	bSavedAttack = false;

	// --- SFX ATTACCO ---
	if (AttackSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetOwner()->GetActorLocation());
	}

	float MaxDuration = 0.0f;
	for (USkeletalMeshComponent* MeshComp : MeshComponents)
	{
		if (MeshComp)
		{
			if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
			{
				float Duration = AnimInstance->Montage_Play(CurrentMontage);
				if (Duration > MaxDuration) MaxDuration = Duration;
			}
		}
	}

	if (MaxDuration > 0.0f)
	{
		if (!bRandomizeAttacks)
		{
			ComboIndex = (ComboIndex + 1) % LightAttackMontages.Num();
		}

		GetWorld()->GetTimerManager().SetTimer(AttackResetTimerHandle, this, &UBrokenDawnMeleeComponent::ResetAttackState, MaxDuration + AttackCooldown, false);
	}
	else
	{
		bIsAttacking = false;
	}

	return MaxDuration;
}

void UBrokenDawnMeleeComponent::EnableComboWindow()
{
	bCanCombo = true;
}

void UBrokenDawnMeleeComponent::DisableComboWindow()
{
	bCanCombo = false;
	if (bSavedAttack)
	{
		bSavedAttack = false;
		bIsAttacking = false;
		PerformLightAttack();
	}
}

void UBrokenDawnMeleeComponent::StartHitCheck(FName SocketStart, FName SocketEnd, float CustomRadius)
{
	bIsCheckingHit = true;
	AlreadyHitActors.Empty();

	CurrentSocketStart = (SocketStart != NAME_None) ? SocketStart : DefaultWeaponSocketStart;
	CurrentSocketEnd = (SocketEnd != NAME_None) ? SocketEnd : DefaultWeaponSocketEnd;
	CurrentTraceRadius = (CustomRadius > 0.0f) ? CustomRadius : HitTraceRadius;
}

void UBrokenDawnMeleeComponent::PerformHitCheck()
{
	if (!bIsCheckingHit) return;

	UMeshComponent* TargetMesh = GetTraceTargetMesh(CurrentSocketStart);
	if (!TargetMesh) return;

	FVector TraceStart = TargetMesh->GetSocketLocation(CurrentSocketStart);
	FVector TraceEnd = TargetMesh->GetSocketLocation(CurrentSocketEnd);

	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	bool bHit = GetWorld()->SweepMultiByObjectType(
		HitResults,
		TraceStart,
		TraceEnd,
		FQuat::Identity,
		ObjectParams,
		FCollisionShape::MakeSphere(CurrentTraceRadius),
		QueryParams
	);

	/*FColor DrawColor = bHit ? FColor::Green : FColor::Red;
	DrawDebugSphere(GetWorld(), TraceStart, CurrentTraceRadius, 12, DrawColor, false, 1.0f);
	DrawDebugSphere(GetWorld(), TraceEnd, CurrentTraceRadius, 12, DrawColor, false, 1.0f);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, DrawColor, false, 1.0f, 0, 1.5f);8*/

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && !AlreadyHitActors.Contains(HitActor))
			{
				if (!bEnableFriendlyFire)
				{
					APawn* OwnerPawn = Cast<APawn>(GetOwner());
					if (!OwnerPawn && GetOwner())
					{
						OwnerPawn = Cast<APawn>(GetOwner()->GetOwner());
					}

					APawn* HitPawn = Cast<APawn>(HitActor);
					if (!HitPawn && HitActor)
					{
						HitPawn = Cast<APawn>(HitActor->GetOwner());
					}

					if (OwnerPawn && HitPawn)
					{
						if (OwnerPawn->IsPlayerControlled() == HitPawn->IsPlayerControlled())
						{
							continue;
						}
					}
				}

				AlreadyHitActors.Add(HitActor);

				//DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 16.0f, 12, FColor::Yellow, false, 2.0f);

				// --- APPLICAZIONE DANNO ---
				// Gli effetti SFX e VFX di risposta al colpo vengono attivati dall'attore colpito nel suo evento TakeDamage/ReceiveAnyDamage
				UGameplayStatics::ApplyPointDamage(
					HitActor,
					BaseDamage,
					(TraceEnd - TraceStart).GetSafeNormal(),
					Hit,
					GetOwner()->GetInstigatorController(),
					GetOwner(),
					UDamageType::StaticClass()
				);
			}
		}
	}
}

void UBrokenDawnMeleeComponent::EndHitCheck()
{
	bIsCheckingHit = false;
	AlreadyHitActors.Empty();
}

void UBrokenDawnMeleeComponent::ResetCombo()
{
	if (!bIsAttacking)
	{
		ComboIndex = 0;
	}
}

void UBrokenDawnMeleeComponent::ResetAttackState()
{
	bIsAttacking = false;
}

UMeshComponent* UBrokenDawnMeleeComponent::GetTraceTargetMesh(FName SocketName) const
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor) return nullptr;

	TArray<UMeshComponent*> AllMeshes;
	OwnerActor->GetComponents<UMeshComponent>(AllMeshes);

	for (UMeshComponent* MeshComp : AllMeshes)
	{
		if (MeshComp && (MeshComp->ComponentHasTag(FName("Weapon")) || MeshComp->ComponentHasTag(FName("WeaponMesh"))))
		{
			if (MeshComp->DoesSocketExist(SocketName))
			{
				return MeshComp;
			}
		}
	}

	for (UMeshComponent* MeshComp : AllMeshes)
	{
		if (MeshComp && MeshComp->DoesSocketExist(SocketName))
		{
			return MeshComp;
		}
	}

	return GetOwnerSkeletalMesh();
}

USkeletalMeshComponent* UBrokenDawnMeleeComponent::GetOwnerSkeletalMesh() const
{
	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	if (!CharacterOwner) return nullptr;

	TArray<USkeletalMeshComponent*> MeshComponents;
	CharacterOwner->GetComponents<USkeletalMeshComponent>(MeshComponents);

	for (USkeletalMeshComponent* MeshComp : MeshComponents)
	{
		if (MeshComp && MeshComp->ComponentHasTag(FName("Mesh1P")))
		{
			return MeshComp;
		}
	}

	for (USkeletalMeshComponent* MeshComp : MeshComponents)
	{
		if (!MeshComp) continue;

		FString CompName = MeshComp->GetName();
		if (CompName.Contains(TEXT("Player")) || CompName.Contains(TEXT("FirstPerson")) ||
			CompName.Contains(TEXT("Mesh1P")) || CompName.Contains(TEXT("1P")))
		{
			if (MeshComp != CharacterOwner->GetMesh())
			{
				return MeshComp;
			}
		}
	}

	for (USkeletalMeshComponent* MeshComp : MeshComponents)
	{
		if (MeshComp && MeshComp != CharacterOwner->GetMesh())
		{
			return MeshComp;
		}
	}

	return CharacterOwner->GetMesh();
}
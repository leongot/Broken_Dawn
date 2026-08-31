#include "BrokenDawnMeleeComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

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
	// Se stiamo già attaccando ma siamo nella finestra di Combo, salviamo il click!
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

	if (SwingSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SwingSound, GetOwner()->GetActorLocation());
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

		// Timer di sicurezza se la combo non prosegue
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
		bIsAttacking = false; // Permette a PerformLightAttack di essere richiamato
		PerformLightAttack(); // Esegue istantaneamente il colpo successivo!
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

	// Visual Debug Helpers
	FColor DrawColor = bHit ? FColor::Green : FColor::Red;
	DrawDebugSphere(GetWorld(), TraceStart, CurrentTraceRadius, 12, DrawColor, false, 1.0f);
	DrawDebugSphere(GetWorld(), TraceEnd, CurrentTraceRadius, 12, DrawColor, false, 1.0f);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, DrawColor, false, 1.0f, 0, 1.5f);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && !AlreadyHitActors.Contains(HitActor))
			{
				// --- FILTRO FUOCO AMICO (Friendly Fire Check) ---
				if (!bEnableFriendlyFire)
				{
					// Recupera il Pawn del proprietario (gestisce sia se il componente è sul Character che su un'Arma)
					APawn* OwnerPawn = Cast<APawn>(GetOwner());
					if (!OwnerPawn && GetOwner())
					{
						OwnerPawn = Cast<APawn>(GetOwner()->GetOwner());
					}

					// Recupera il Pawn dell'attore colpito (gestisce anche mesh/accessori agganciati al nemico)
					APawn* HitPawn = Cast<APawn>(HitActor);
					if (!HitPawn && HitActor)
					{
						HitPawn = Cast<APawn>(HitActor->GetOwner());
					}

					if (OwnerPawn && HitPawn)
					{
						// Se entrambi sono controllati dal Giocatore o entrambi sono AI/Nemici, ignora il colpo
						if (OwnerPawn->IsPlayerControlled() == HitPawn->IsPlayerControlled())
						{
							continue;
						}
					}
				}

				AlreadyHitActors.Add(HitActor);

				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 16.0f, 12, FColor::Yellow, false, 2.0f);

				// --- 1. RIPRODUZIONE AUDIO IMPATTO ---
				if (HitSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, HitSound, Hit.ImpactPoint);
				}

				// --- 2. SPAWN PARTICELLE SUL PUNTO DI IMPATTO ---
				if (HitNiagaraEffect)
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(
						GetWorld(),
						HitNiagaraEffect,
						Hit.ImpactPoint,
						Hit.ImpactNormal.Rotation()
					);
				}
				else if (HitParticleEffect)
				{
					UGameplayStatics::SpawnEmitterAtLocation(
						GetWorld(),
						HitParticleEffect,
						Hit.ImpactPoint,
						Hit.ImpactNormal.Rotation()
					);
				}

				// --- 3. APPLICAZIONE DANNO ---
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
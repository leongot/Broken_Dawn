#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BrokenDawnMeleeComponent.generated.h"

class UAnimMontage;
class USkeletalMeshComponent;
class UMeshComponent;
class USoundBase;
class UParticleSystem;
class UNiagaraSystem;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BROKENDAWN_API UBrokenDawnMeleeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBrokenDawnMeleeComponent();

	UFUNCTION(BlueprintCallable, Category = "Melee")
	void EnableComboWindow();

	UFUNCTION(BlueprintCallable, Category = "Melee")
	void DisableComboWindow();

protected:
	virtual void BeginPlay() override;

	bool bCanCombo = false;
	bool bSavedAttack = false;

	// Configurazione Attacco
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Config")
	bool bRandomizeAttacks = false;

	/** Permette il fuoco amico (se true, i nemici possono colpirsi tra loro e il player colpire gli alleati) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Config")
	bool bEnableFriendlyFire = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Config")
	TArray<UAnimMontage*> LightAttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Config")
	float BaseDamage = 35.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Config")
	float StaminaCostPerAttack = 15.0f;

	/** Tempo di ricarica/pausa aggiuntivo (in secondi) dopo la fine dell'animazione prima di poter riattaccare */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Config")
	float AttackCooldown = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Config")
	float HitTraceRadius = 12.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Config")
	FName DefaultWeaponSocketStart = "WeaponTraceStart";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Config")
	FName DefaultWeaponSocketEnd = "WeaponTraceEnd";

	// --- EFFETTI VISIVI E AUDIO (VFX / SFX) ---

	/** Suono riprodotto all'impatto con un nemico/bersaglio */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Effects")
	USoundBase* HitSound;

	/** Suono riprodotto durante il fendente nell'aria */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Effects")
	USoundBase* SwingSound;

	/** Effetto particellare Cascade (legacy) generato sul punto d'impatto */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Effects")
	UParticleSystem* HitParticleEffect;

	/** Effetto particellare Niagara (UE5 Standard) generato sul punto d'impatto */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Effects")
	UNiagaraSystem* HitNiagaraEffect;

	// Stato Interno
	bool bIsAttacking = false;
	bool bIsCheckingHit = false;
	int32 ComboIndex = 0;
	TSet<AActor*> AlreadyHitActors;
	FTimerHandle AttackResetTimerHandle;

	// Sockets correnti impostati dall'AnimNotify
	FName CurrentSocketStart;
	FName CurrentSocketEnd;
	float CurrentTraceRadius = -1.0f;

public:
	/** Esegue l'attacco leggero e restituisce la durata dell'animazione in secondi (0.0f se fallisce o è in cooldown) */
	UFUNCTION(BlueprintCallable, Category = "Melee")
	float PerformLightAttack();

	UFUNCTION(BlueprintCallable, Category = "Melee")
	void StartHitCheck(FName SocketStart = NAME_None, FName SocketEnd = NAME_None, float CustomRadius = -1.0f);

	UFUNCTION(BlueprintCallable, Category = "Melee")
	void PerformHitCheck();

	UFUNCTION(BlueprintCallable, Category = "Melee")
	void EndHitCheck();

	UFUNCTION(BlueprintCallable, Category = "Melee")
	void ResetCombo();

	UFUNCTION(BlueprintCallable, Category = "Melee")
	void ResetAttackState();

private:
	USkeletalMeshComponent* GetOwnerSkeletalMesh() const;
	UMeshComponent* GetTraceTargetMesh(FName SocketName) const;
};
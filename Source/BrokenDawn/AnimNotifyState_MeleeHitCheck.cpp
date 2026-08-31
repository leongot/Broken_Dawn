#include "AnimNotifyState_MeleeHitCheck.h"
#include "BrokenDawnMeleeComponent.h"
#include "GameFramework/Actor.h"

void UAnimNotifyState_MeleeHitCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		UBrokenDawnMeleeComponent* MeleeComp = MeshComp->GetOwner()->FindComponentByClass<UBrokenDawnMeleeComponent>();
		if (MeleeComp)
		{
			MeleeComp->StartHitCheck();
		}
	}
}

void UAnimNotifyState_MeleeHitCheck::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		UBrokenDawnMeleeComponent* MeleeComp = MeshComp->GetOwner()->FindComponentByClass<UBrokenDawnMeleeComponent>();
		if (MeleeComp)
		{
			MeleeComp->PerformHitCheck();
		}
	}
}

void UAnimNotifyState_MeleeHitCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		UBrokenDawnMeleeComponent* MeleeComp = MeshComp->GetOwner()->FindComponentByClass<UBrokenDawnMeleeComponent>();
		if (MeleeComp)
		{
			MeleeComp->EndHitCheck();
		}
	}
}
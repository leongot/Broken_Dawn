#include "AnimNotifyState_ComboWindow.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Components/SkeletalMeshComponent.h"
#include "BrokenDawnMeleeComponent.h"

void UAnimNotifyState_ComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (UBrokenDawnMeleeComponent* MeleeComp = MeshComp->GetOwner()->FindComponentByClass<UBrokenDawnMeleeComponent>())
		{
			MeleeComp->EnableComboWindow();
		}
	}
}

void UAnimNotifyState_ComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (UBrokenDawnMeleeComponent* MeleeComp = MeshComp->GetOwner()->FindComponentByClass<UBrokenDawnMeleeComponent>())
		{
			MeleeComp->DisableComboWindow();
		}
	}
}
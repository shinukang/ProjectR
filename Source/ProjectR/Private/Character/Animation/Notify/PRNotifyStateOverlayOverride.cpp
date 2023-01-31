#include "Character/Animation/Notify/PRNotifyStateOverlayOverride.h"
#include "Character/PRBaseCharacter.h"
#include "Character/Animation/PRCharacterAnimInstance.h"

void UPRNotifyStateOverlayOverride::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                 float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	APRBaseCharacter* Character = Cast<APRBaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->SetOverlayOverrideState(OverlayOverrideState);
	}
}

void UPRNotifyStateOverlayOverride::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	APRBaseCharacter* Character = Cast<APRBaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->SetOverlayOverrideState(0);
	}
}

FString UPRNotifyStateOverlayOverride::GetNotifyName_Implementation() const
{
	FString Name(TEXT("Overlay Override State: "));
	Name.Append(FString::FromInt(OverlayOverrideState));
	return Name;
}

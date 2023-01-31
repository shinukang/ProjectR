#include "Character/Animation/Notify/PRNotifyStateMovementAction.h"

#include "Character/PRBaseCharacter.h"

void UPRNotifyStateMovementAction::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	APRBaseCharacter* BaseCharacter = Cast<APRBaseCharacter>(MeshComp->GetOwner());
	if (BaseCharacter)
	{
		BaseCharacter->SetMovementAction(MovementAction);
	}
}

void UPRNotifyStateMovementAction::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	APRBaseCharacter* BaseCharacter = Cast<APRBaseCharacter>(MeshComp->GetOwner());
	if (BaseCharacter && BaseCharacter->GetMovementAction() == MovementAction)
	{
		BaseCharacter->SetMovementAction(EPRMovementAction::None);
	}
}

FString UPRNotifyStateMovementAction::GetNotifyName_Implementation() const
{
	FString Name(TEXT("Movement Action: "));
	Name.Append(GetEnumerationToString(MovementAction));
	return Name;
}

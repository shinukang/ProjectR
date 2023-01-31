#include "Character/Animation/Notify/PRAnimNotifyGroundedEntryState.h"

#include "Character/PRBaseCharacter.h"
#include "Character/Animation/PRCharacterAnimInstance.h"

void UPRAnimNotifyGroundedEntryState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	APRBaseCharacter* Character = Cast<APRBaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->SetGroundedEntryState(GroundedEntryState);
	}
}

FString UPRAnimNotifyGroundedEntryState::GetNotifyName_Implementation() const
{
	FString Name(TEXT("Grounded Entry State: "));
	Name.Append(GetEnumerationToString(GroundedEntryState));
	return Name;
}

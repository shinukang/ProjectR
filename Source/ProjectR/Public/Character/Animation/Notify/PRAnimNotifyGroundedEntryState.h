#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Library/PRCharacterEnumLibrary.h"

#include "PRAnimNotifyGroundedEntryState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UPRAnimNotifyGroundedEntryState : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	EPRGroundedEntryState GroundedEntryState = EPRGroundedEntryState::None;
};

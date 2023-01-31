#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Library/PRCharacterEnumLibrary.h"

#include "PRNotifyStateEarlyBlendOut.generated.h"

/**
 * Character early blend out anim state
 */
UCLASS()
class PROJECTR_API UPRNotifyStateEarlyBlendOut : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	                        const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	TObjectPtr<UAnimMontage> ThisMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	float BlendOutTime = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	bool bCheckMovementState = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	EPRMovementState MovementStateEquals = EPRMovementState::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	bool bCheckStance = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	EPRStance StanceEquals = EPRStance::Standing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	bool bCheckMovementInput = false;
};

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeBase.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PRAnimNotifyCameraShake.generated.h"

class UCameraShakeBase;

/**
 * Generic camera shake animation notify for pawns with controller enabled
 */
UCLASS()
class PROJECTR_API UPRAnimNotifyCameraShake : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	TSubclassOf<UCameraShakeBase> ShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	float Scale = 1.0f;
};

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Library/PRCharacterEnumLibrary.h"

#include "PRPlayerCameraBehavior.generated.h"

class APRBaseCharacter;
class APRPlayerController;

/**
 * Main class for player camera movement behavior
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTR_API UPRPlayerCameraBehavior : public UAnimInstance
{
	GENERATED_BODY()

public:
	void SetRotationMode(EPRRotationMode RotationMode);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	EPRMovementState MovementState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	EPRMovementAction MovementAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	bool bLookingDirection = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	bool bVelocityDirection = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	bool bAiming = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	EPRGait Gait;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	EPRStance Stance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	EPRViewMode ViewMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	bool bRightShoulder = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Read Only Data|Character Information")
	bool bDebugView = false;
};

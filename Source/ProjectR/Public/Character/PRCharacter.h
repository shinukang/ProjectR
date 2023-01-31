#pragma once

#include "CoreMinimal.h"
#include "Character/PRBaseCharacter.h"
#include "PRCharacter.generated.h"

/**
 * Specialized character class, with additional features like held object etc.
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTR_API APRCharacter : public APRBaseCharacter
{
	GENERATED_BODY()

public:
	APRCharacter(const FObjectInitializer& ObjectInitializer);

	/** Implemented on BP to update held objects */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PR|HeldObject")
	void UpdateHeldObject();

	UFUNCTION(BlueprintCallable, Category = "PR|HeldObject")
	void ClearHeldObject();

	UFUNCTION(BlueprintCallable, Category = "PR|HeldObject")
	void AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh,
	                  class UClass* NewAnimClass, bool bLeftHand, FVector Offset);

	virtual void RagdollStart() override;

	virtual void RagdollEnd() override;

	virtual ECollisionChannel GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius) override;

	virtual FTransform GetThirdPersonPivotTarget() override;

	virtual FVector GetFirstPersonCameraTarget() override;

protected:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void OnOverlayStateChanged(EPROverlayState PreviousState) override;

	/** Implement on BP to update animation states of held objects */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PR|HeldObject")
	void UpdateHeldObjectAnimations();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Component")
	TObjectPtr<USceneComponent> HeldObjectRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Component")
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Component")
	TObjectPtr<UStaticMeshComponent> StaticMesh = nullptr;

private:
	bool bNeedsColorReset = false;
};

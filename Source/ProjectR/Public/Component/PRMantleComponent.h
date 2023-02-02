#pragma once

#include "CoreMinimal.h"

#include "Character/PRBaseCharacter.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "PRMantleComponent.generated.h"

// forward declarations
class UPRDebugComponent;


UCLASS(Blueprintable, BlueprintType)
class PROJECTR_API UPRMantleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPRMantleComponent();

	UFUNCTION(BlueprintCallable, Category = "PR|Mantle System")
	bool MantleCheck(const FPRMantleTraceSettings& TraceSettings,
	                 EDrawDebugTrace::Type DebugType);

	UFUNCTION(BlueprintCallable, Category = "PR|Mantle System")
	void MantleStart(float MantleHeight, const FPRComponentAndTransform& MantleLedgeWS,
	                EPRMantleType MantleType);

	UFUNCTION(BlueprintCallable, Category = "PR|Mantle System")
	void MantleUpdate(float BlendIn);

	UFUNCTION(BlueprintCallable, Category = "PR|Mantle System")
	void MantleEnd();

	UFUNCTION(BlueprintCallable, Category = "PR|Mantle System")
	void OnOwnerJumpInput();

	UFUNCTION(BlueprintCallable, Category = "PR|Mantle System")
	void OnOwnerRagdollStateChanged(bool bRagdollState);

	/** Implement on BP to get correct mantle parameter set according to character state */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PR|Mantle System")
	FPRMantleAsset GetMantleAsset(EPRMantleType MantleType, EPROverlayState CurrentOverlayState);

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// Called when the game starts
	virtual void BeginPlay() override;

	/** Mantling*/
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "PR|Mantle System")
	void Server_MantleStart(float MantleHeight, const FPRComponentAndTransform& MantleLedgeWS,
	                        EPRMantleType MantleType);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "PR|Mantle System")
	void Multicast_MantleStart(float MantleHeight, const FPRComponentAndTransform& MantleLedgeWS,
	                           EPRMantleType MantleType);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Mantle System")
	TObjectPtr<UTimelineComponent> MantleTimeline = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PR|Mantle System")
	FPRMantleTraceSettings GroundedTraceSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PR|Mantle System")
	FPRMantleTraceSettings AutomaticTraceSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PR|Mantle System")
	FPRMantleTraceSettings FallingTraceSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PR|Mantle System")
	TObjectPtr<UCurveFloat> MantleTimelineCurve;

	static FName NAME_IgnoreOnlyPawn;
	/** Profile to use to detect objects we allow mantling */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PR|Mantle System")
	FName MantleObjectDetectionProfile = NAME_IgnoreOnlyPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PR|Mantle System")
	TEnumAsByte<ECollisionChannel> WalkableSurfaceDetectionChannel = ECC_Visibility;

	UPROPERTY(BlueprintReadOnly, Category = "PR|Mantle System")
	FPRMantleParams MantleParams;

	UPROPERTY(BlueprintReadOnly, Category = "PR|Mantle System")
	FPRComponentAndTransform MantleLedgeLS;

	UPROPERTY(BlueprintReadOnly, Category = "PR|Mantle System")
	FTransform MantleTarget = FTransform::Identity;

	UPROPERTY(BlueprintReadOnly, Category = "PR|Mantle System")
	FTransform MantleActualStartOffset = FTransform::Identity;

	UPROPERTY(BlueprintReadOnly, Category = "PR|Mantle System")
	FTransform MantleAnimatedStartOffset = FTransform::Identity;

	/** If a dynamic object has a velocity bigger than this value, do not start mantle */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PR|Mantle System")
	float AcceptableVelocityWhileMantling = 10.0f;

private:
	UPROPERTY()
	TObjectPtr<APRBaseCharacter> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<UPRDebugComponent> PRDebugComponent = nullptr;
};

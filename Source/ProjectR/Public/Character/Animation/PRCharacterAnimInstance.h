#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Library/PRAnimationStructLibrary.h"
#include "Library/PRStructEnumLibrary.h"
#include "PRCharacterAnimInstance.generated.h"

// forward declarations
class UPRDebugComponent;
class APRBaseCharacter;
class UCurveFloat;
class UAnimSequence;
class UCurveVector;

/**
 * Main anim instance class for character
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTR_API UPRCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "PR|Animation")
	void PlayTransition(const FPRDynamicMontageParams& Parameters);

	UFUNCTION(BlueprintCallable, Category = "PR|Animation")
	void PlayTransitionChecked(const FPRDynamicMontageParams& Parameters);

	UFUNCTION(BlueprintCallable, Category = "PR|Animation")
	void PlayDynamicTransition(float ReTriggerDelay, FPRDynamicMontageParams Parameters);

	UFUNCTION(BlueprintCallable, Category = "PR|Event")
	void OnJumped();

	UFUNCTION(BlueprintCallable, Category = "PR|Event")
	void OnPivot();

protected:

	UFUNCTION(BlueprintCallable, Category = "PR|Grounded")
	void SetTrackedHipsDirection(EPRHipsDirection HipsDirection)
	{
		Grounded.TrackedHipsDirection = HipsDirection;
	}

	UFUNCTION(BlueprintCallable, Category = "PR|Grounded")
	void SetGroundedEntryState(EPRGroundedEntryState NewState)
	{
		GroundedEntryState = NewState;
	}

	/** Enable Movement Animations if IsMoving and HasMovementInput, or if the Speed is greater than 150. */
	UFUNCTION(BlueprintCallable, Category = "PR|Grounded")
	bool ShouldMoveCheck() const;

	/** Only perform a Rotate In Place Check if the character is Aiming or in First Person. */
	UFUNCTION(BlueprintCallable, Category = "PR|Grounded")
	bool CanRotateInPlace() const;

	/**
	 * Only perform a Turn In Place check if the character is looking toward the camera in Third Person,
	 * and if the "Enable Transition" curve is fully weighted. The Enable_Transition curve is modified within certain
	 * states of the AnimBP so that the character can only turn while in those states..
	 */
	UFUNCTION(BlueprintCallable, Category = "PR|Grounded")
	bool CanTurnInPlace() const;

	/**
	 * Only perform a Dynamic Transition check if the "Enable Transition" curve is fully weighted.
	 * The Enable_Transition curve is modified within certain states of the AnimBP so
	 * that the character can only transition while in those states.
	 */
	UFUNCTION(BlueprintCallable, Category = "PR|Grounded")
	bool CanDynamicTransition() const;

private:
	void PlayDynamicTransitionDelay();

	void OnJumpedDelay();

	void OnPivotDelay();

	/** Update Values */

	void UpdateAimingValues(float DeltaSeconds);

	void UpdateLayerValues();

	void UpdateFootIK(float DeltaSeconds);

	void UpdateMovementValues(float DeltaSeconds);

	void UpdateRotationValues();

	void UpdateInAirValues(float DeltaSeconds);

	void UpdateRagdollValues();

	/** Foot IK */

	void SetFootLocking(float DeltaSeconds, FName EnableFootIKCurve, FName FootLockCurve, FName IKFootBone,
                          float& CurFootLockAlpha, bool& UseFootLockCurve,
                          FVector& CurFootLockLoc, FRotator& CurFootLockRot);

	void SetFootLockOffsets(float DeltaSeconds, FVector& LocalLoc, FRotator& LocalRot);

	void SetPelvisIKOffset(float DeltaSeconds, FVector FootOffsetLTarget, FVector FootOffsetRTarget);

	void ResetIKOffsets(float DeltaSeconds);

	void SetFootOffsets(float DeltaSeconds, FName EnableFootIKCurve, FName IKFootBone, FName RootBone,
                          FVector& CurLocationTarget, FVector& CurLocationOffset, FRotator& CurRotationOffset);

	/** Grounded */

	void RotateInPlaceCheck();

	void TurnInPlaceCheck(float DeltaSeconds);

	void DynamicTransitionCheck();

	FPRVelocityBlend CalculateVelocityBlend() const;

	void TurnInPlace(FRotator TargetRotation, float PlayRateScale, float StartTime, bool OverrideCurrent);

	/** Movement */

	FVector CalculateRelativeAccelerationAmount() const;

	float CalculateStrideBlend() const;

	float CalculateWalkRunBlend() const;

	float CalculateStandingPlayRate() const;

	float CalculateDiagonalScaleAmount() const;

	float CalculateCrouchingPlayRate() const;

	float CalculateLandPrediction() const;

	FPRLeanAmount CalculateAirLeanAmount() const;

	EPRMovementDirection CalculateMovementDirection() const;

	/** Util */

	float GetAnimCurveClamped(const FName& Name, float Bias, float ClampMin, float ClampMax) const;

public:
	/** References */
	UPROPERTY(BlueprintReadOnly, Category = "Read Only Data|Character Information")
	TObjectPtr<APRBaseCharacter> Character = nullptr;

	/** Character Information */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information", Meta = (
		ShowOnlyInnerProperties))
	FPRAnimCharacterInformation CharacterInformation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	FPRMovementState MovementState = EPRMovementState::None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	FPRMovementAction MovementAction = EPRMovementAction::None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	FPRRotationMode RotationMode = EPRRotationMode::VelocityDirection;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	FPRGait Gait = EPRGait::Walking;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	FPRStance Stance = EPRStance::Standing;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	FPROverlayState OverlayState = EPROverlayState::Default;

	/** Anim Graph - Grounded */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded", Meta = (
		ShowOnlyInnerProperties))
	FPRAnimGraphGrounded Grounded;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	FPRVelocityBlend VelocityBlend;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	FPRLeanAmount LeanAmount;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	FVector RelativeAccelerationAmount = FVector::ZeroVector;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	FPRGroundedEntryState GroundedEntryState = EPRGroundedEntryState::None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	FPRMovementDirection MovementDirection = EPRMovementDirection::Forward;

	/** Anim Graph - In Air */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - In Air", Meta = (
		ShowOnlyInnerProperties))
	FPRAnimGraphInAir InAir;

	/** Anim Graph - Aiming Values */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Aiming Values", Meta = (
		ShowOnlyInnerProperties))
	FPRAnimGraphAimingValues AimingValues;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Aiming Values")
	FVector2D SmoothedAimingAngle = FVector2D::ZeroVector;

	/** Anim Graph - Ragdoll */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Ragdoll")
	float FlailRate = 0.0f;

	/** Anim Graph - Layer Blending */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending", Meta = (
		ShowOnlyInnerProperties))
	FPRAnimGraphLayerBlending LayerBlendingValues;

	/** Anim Graph - Foot IK */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Foot IK", Meta = (
		ShowOnlyInnerProperties))
	FPRAnimGraphFootIK FootIKValues;

	/** Turn In Place */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Turn In Place", Meta = (
		ShowOnlyInnerProperties))
	FPRAnimTurnInPlace TurnInPlaceValues;

	/** Rotate In Place */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Rotate In Place", Meta = (
		    ShowOnlyInnerProperties))
	FPRAnimRotateInPlace RotateInPlace;

	/** Configuration */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Main Configuration", Meta = (
		ShowOnlyInnerProperties))
	FPRAnimConfiguration Config;

	/** Blend Curves */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves")
	TObjectPtr<UCurveFloat> DiagonalScaleAmountCurve = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves")
	TObjectPtr<UCurveFloat> StrideBlend_N_Walk = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves")
	TObjectPtr<UCurveFloat> StrideBlend_N_Run = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves")
	TObjectPtr<UCurveFloat> StrideBlend_C_Walk = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves")
	TObjectPtr<UCurveFloat> LandPredictionCurve = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves")
	TObjectPtr<UCurveFloat> LeanInAirCurve = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves")
	TObjectPtr<UCurveVector> YawOffset_FB = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves")
	TObjectPtr<UCurveVector> YawOffset_LR = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Dynamic Transition")
	TObjectPtr<UAnimSequenceBase> TransitionAnim_R = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Dynamic Transition")
	TObjectPtr<UAnimSequenceBase> TransitionAnim_L = nullptr;

	/** IK Bone Names */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Anim Graph - Foot IK")
	FName IkFootL_BoneName = FName(TEXT("ik_foot_l"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Anim Graph - Foot IK")
	FName IkFootR_BoneName = FName(TEXT("ik_foot_r"));

private:
	FTimerHandle OnPivotTimer;

	FTimerHandle PlayDynamicTransitionTimer;

	FTimerHandle OnJumpedTimer;

	bool bCanPlayDynamicTransition = true;

	UPROPERTY()
	TObjectPtr<UPRDebugComponent> PRDebugComponent = nullptr;
};

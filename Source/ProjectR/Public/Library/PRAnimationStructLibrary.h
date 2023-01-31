#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Animation/AnimSequenceBase.h"
#include "PRCharacterEnumLibrary.h"


#include "PRAnimationStructLibrary.generated.h"


USTRUCT(BlueprintType)
struct FPRDynamicMontageParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Dynamic Transition")
	TObjectPtr<UAnimSequenceBase> Animation = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Dynamic Transition")
	float BlendInTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Dynamic Transition")
	float BlendOutTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Dynamic Transition")
	float PlayRate = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Dynamic Transition")
	float StartTime = 0.0f;
};

USTRUCT(BlueprintType)
struct FPRLeanAmount
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Grounded")
	float LR = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Grounded")
	float FB = 0.0f;
};

USTRUCT(BlueprintType)
struct FPRVelocityBlend
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Grounded")
	float F = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Grounded")
	float B = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Grounded")
	float L = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Grounded")
	float R = 0.0f;
};

USTRUCT(BlueprintType)
struct FPRTurnInPlaceAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Turn In Place")
	TObjectPtr<UAnimSequenceBase> Animation = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Turn In Place")
	float AnimatedAngle = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Turn In Place")
	FName SlotName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Turn In Place")
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Turn In Place")
	bool ScaleTurnAngle = true;
};

USTRUCT(BlueprintType)
struct FPRAnimCharacterInformation
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Character Information")
	FRotator AimingRotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Character Information")
	FRotator CharacterActorRotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Character Information")
	FVector Velocity;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Character Information")
	FVector RelativeVelocityDirection;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Character Information")
	FVector Acceleration;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Character Information")
	FVector MovementInput;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Character Information")
	bool bIsMoving = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Character Information")
	bool bHasMovementInput = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Character Information")
	float Speed = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Character Information")
	float MovementInputAmount = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Character Information")
	float AimYawRate = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Character Information")
	float ZoomAmount = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Character Information")
	EPRMovementState PrevMovementState = EPRMovementState::None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Character Information")
	EPRViewMode ViewMode = EPRViewMode::ThirdPerson;
};

USTRUCT(BlueprintType)
struct FPRAnimGraphGrounded
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PR|Anim Graph - Grounded")
	EPRHipsDirection TrackedHipsDirection = EPRHipsDirection::F;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Grounded")
	bool bShouldMove = false; // Should be false initially

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Grounded")
	bool bRotateL = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Grounded")
	bool bRotateR = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PR|Anim Graph - Grounded")
	bool bPivot = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Grounded")
	float RotateRate = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Grounded")
	float RotationScale = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Grounded")
	float DiagonalScaleAmount = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Grounded")
	float WalkRunBlend = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Grounded")
	float StandingPlayRate = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Grounded")
	float CrouchingPlayRate = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Grounded")
	float StrideBlend = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Grounded")
	float FYaw = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Grounded")
	float BYaw = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Grounded")
	float LYaw = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Grounded")
	float RYaw = 0.0f;
};

USTRUCT(BlueprintType)
struct FPRAnimGraphInAir
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - In Air")
	bool bJumped = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - In Air")
	float JumpPlayRate = 1.2f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - In Air")
	float FallSpeed = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - In Air")
	float LandPrediction = 1.0f;
};

USTRUCT(BlueprintType)
struct FPRAnimGraphAimingValues
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Aiming Values")
	FRotator SmoothedAimingRotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Aiming Values")
	FRotator SpineRotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Aiming Values")
	FVector2D AimingAngle;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Aiming Values")
	float AimSweepTime = 0.5f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Aiming Values")
	float InputYawOffsetTime = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Aiming Values")
	float ForwardYawTime = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Aiming Values")
	float LeftYawTime = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Aiming Values")
	float RightYawTime = 0.0f;
};

USTRUCT(BlueprintType)
struct FPRAnimGraphLayerBlending
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	int32 OverlayOverrideState = 0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float EnableAimOffset = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float BasePose_N = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float BasePose_CLF = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float Arm_L = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float Arm_L_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float Arm_L_LS = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float Arm_L_MS = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float Arm_R = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float Arm_R_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float Arm_R_LS = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float Arm_R_MS = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float Hand_L = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float Hand_R = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float Legs = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float Legs_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float Pelvis = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float Pelvis_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float Spine = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float Spine_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float Head = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float Head_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float EnableHandIK_L = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Layer Blending")
	float EnableHandIK_R = 1.0f;
};

USTRUCT(BlueprintType)
struct FPRAnimGraphFootIK
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Foot IK")
	float FootLock_L_Alpha = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Foot IK")
	float FootLock_R_Alpha = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Foot IK")
	bool UseFootLockCurve_L;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Foot IK")
	bool UseFootLockCurve_R;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Foot IK")
	FVector FootLock_L_Location;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Foot IK")
	FVector TargetFootLock_R_Location;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Foot IK")
	FVector FootLock_R_Location;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Foot IK")
	FRotator TargetFootLock_L_Rotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Foot IK")
	FRotator FootLock_L_Rotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Foot IK")
	FRotator TargetFootLock_R_Rotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Foot IK")
	FRotator FootLock_R_Rotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Foot IK")
	FVector FootOffset_L_Location;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Foot IK")
	FVector FootOffset_R_Location;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Foot IK")
	FRotator FootOffset_L_Rotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Foot IK")
	FRotator FootOffset_R_Rotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Foot IK")
	FVector PelvisOffset;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PR|Anim Graph - Foot IK")
	float PelvisAlpha = 0.0f;
};

USTRUCT(BlueprintType)
struct FPRAnimTurnInPlace
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Turn In Place")
	float TurnCheckMinAngle = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Turn In Place")
	float Turn180Threshold = 130.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Turn In Place")
	float AimYawRateLimit = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Turn In Place")
	float ElapsedDelayTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Turn In Place")
	float MinAngleDelay = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Turn In Place")
	float MaxAngleDelay = 0.75f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Turn In Place")
	FPRTurnInPlaceAsset N_TurnIP_L90;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Turn In Place")
	FPRTurnInPlaceAsset N_TurnIP_R90;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Turn In Place")
	FPRTurnInPlaceAsset N_TurnIP_L180;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Turn In Place")
	FPRTurnInPlaceAsset N_TurnIP_R180;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Turn In Place")
	FPRTurnInPlaceAsset CLF_TurnIP_L90;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Turn In Place")
	FPRTurnInPlaceAsset CLF_TurnIP_R90;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Turn In Place")
	FPRTurnInPlaceAsset CLF_TurnIP_L180;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Turn In Place")
	FPRTurnInPlaceAsset CLF_TurnIP_R180;
};

USTRUCT(BlueprintType)
struct FPRAnimRotateInPlace
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Rotate In Place")
	float RotateMinThreshold = -50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Rotate In Place")
	float RotateMaxThreshold = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Rotate In Place")
	float AimYawRateMinRange = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Rotate In Place")
	float AimYawRateMaxRange = 270.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Rotate In Place")
	float MinPlayRate = 1.15f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Rotate In Place")
	float MaxPlayRate = 3.0f;
};

USTRUCT(BlueprintType)
struct FPRAnimConfiguration
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Main Configuration")
	float AnimatedWalkSpeed = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Main Configuration")
	float AnimatedRunSpeed = 350.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Main Configuration")
	float AnimatedSprintSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Main Configuration")
	float AnimatedCrouchSpeed = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Main Configuration")
	float VelocityBlendInterpSpeed = 12.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Main Configuration")
	float GroundedLeanInterpSpeed = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Main Configuration")
	float InAirLeanInterpSpeed = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Main Configuration")
	float SmoothedAimingRotationInterpSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Main Configuration")
	float InputYawOffsetInterpSpeed = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Main Configuration")
	float TriggerPivotSpeedLimit = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Main Configuration")
	float FootHeight = 13.5f;

	/** Threshold value for activating dynamic transition on various animations */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Main Configuration")
	float DynamicTransitionThreshold = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Main Configuration")
	float IK_TraceDistanceAboveFoot = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Main Configuration")
	float IK_TraceDistanceBelowFoot = 45.0f;
};

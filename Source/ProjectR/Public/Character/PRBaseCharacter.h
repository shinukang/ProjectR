#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Library/PRCharacterEnumLibrary.h"
#include "Library/PRCharacterStructLibrary.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"

#include "PRBaseCharacter.generated.h"

// forward declarations
class UPRDebugComponent;
class UAnimMontage;
class UPRPlayerCameraBehavior;
enum class EVisibilityBasedAnimTickOption : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpPressedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJumpedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRagdollStateChangedSignature, bool, bRagdollState);

/*
 * Base character class
 */
UCLASS(BlueprintType)
class PROJECTR_API APRBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APRBaseCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "PR|Movement")
		FORCEINLINE class UPRCharacterMovementComponent* GetMyMovementComponent() const
	{
		return MyCharacterMovementComponent;
	}

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Ragdoll System */

	/** Implement on BP to get required get up animation according to character's state */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "PR|Ragdoll System")
		UAnimMontage* GetGetUpAnimation(bool bRagdollFaceUpState);

	UFUNCTION(BlueprintCallable, Category = "PR|Ragdoll System")
		virtual void RagdollStart();

	UFUNCTION(BlueprintCallable, Category = "PR|Ragdoll System")
		virtual void RagdollEnd();

	UFUNCTION(BlueprintCallable, Server, Unreliable, Category = "PR|Ragdoll System")
		void Server_SetMeshLocationDuringRagdoll(FVector MeshLocation);

	/** Character States */

	UFUNCTION(BlueprintCallable, Category = "PR|Character States")
		void SetMovementState(EPRMovementState NewState, bool bForce = false);

	UFUNCTION(BlueprintGetter, Category = "PR|Character States")
		EPRMovementState GetMovementState() const { return MovementState; }

	UFUNCTION(BlueprintGetter, Category = "PR|Character States")
		EPRMovementState GetPrevMovementState() const { return PrevMovementState; }

	UFUNCTION(BlueprintCallable, Category = "PR|Character States")
		void SetMovementAction(EPRMovementAction NewAction, bool bForce = false);

	UFUNCTION(BlueprintGetter, Category = "PR|Character States")
		EPRMovementAction GetMovementAction() const { return MovementAction; }

	UFUNCTION(BlueprintCallable, Category = "PR|Character States")
		void SetStance(EPRStance NewStance, bool bForce = false);

	UFUNCTION(BlueprintGetter, Category = "PR|Character States")
		EPRStance GetStance() const { return Stance; }

	UFUNCTION(BlueprintCallable, Category = "PR|Character States")
		void SetOverlayOverrideState(int32 NewState);

	UFUNCTION(BlueprintGetter, Category = "PR|Character States")
		int32 GetOverlayOverrideState() const { return OverlayOverrideState; }

	UFUNCTION(BlueprintCallable, Category = "PR|Character States")
		void SetGait(EPRGait NewGait, bool bForce = false);

	UFUNCTION(BlueprintGetter, Category = "PR|Character States")
		EPRGait GetGait() const { return Gait; }

	UFUNCTION(BlueprintGetter, Category = "PR|CharacterStates")
		EPRGait GetDesiredGait() const { return DesiredGait; }

	UFUNCTION(BlueprintCallable, Category = "PR|Character States")
		void SetRotationMode(EPRRotationMode NewRotationMode, bool bForce = false);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "PR|Character States")
		void Server_SetRotationMode(EPRRotationMode NewRotationMode, bool bForce);

	UFUNCTION(BlueprintGetter, Category = "PR|Character States")
		EPRRotationMode GetRotationMode() const { return RotationMode; }

	UFUNCTION(BlueprintCallable, Category = "PR|Character States")
		void SetViewMode(EPRViewMode NewViewMode, bool bForce = false);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "PR|Character States")
		void Server_SetViewMode(EPRViewMode NewViewMode, bool bForce);

	UFUNCTION(BlueprintGetter, Category = "PR|Character States")
		EPRViewMode GetViewMode() const { return ViewMode; }

	UFUNCTION(BlueprintCallable, Category = "PR|Character States")
		void SetOverlayState(EPROverlayState NewState, bool bForce = false);

	UFUNCTION(BlueprintCallable, Category = "PR|Character States")
		void SetNeedToResetOverlayState(bool bNeedToReset);

	UFUNCTION(BlueprintCallable, Category = "PR|Character States")
		void SetIsReloading(bool bIsReload);

	UFUNCTION(BlueprintCallable, Category = "PR|Character States")
		void SetIsEquipping(bool bIsEquip);

	UFUNCTION(BlueprintCallable, Category = "PR|Character States")
		void SetGroundedEntryState(EPRGroundedEntryState NewState);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "PR|Character States")
		void Server_SetOverlayState(EPROverlayState NewState, bool bForce);

	UFUNCTION(BlueprintGetter, Category = "PR|Character States")
		EPROverlayState GetOverlayState() const { return OverlayState; }

	UFUNCTION(BlueprintGetter, Category = "PR|Character States")
		bool GetNeedToResetOverlayState() const { return bNeedToResetOverlayState;  }

	UFUNCTION(BlueprintGetter, Category = "PR|Character States")
		bool GetIsReloading() const { return bIsReloading; }

	UFUNCTION(BlueprintGetter, Category = "PR|Character States")
		bool GetIsEquipping() const { return bIsEquipping; }

	UFUNCTION(BlueprintGetter, Category = "PR|Character States")
		EPRGroundedEntryState GetGroundedEntryState() const { return GroundedEntryState; }

	/** Landed, Jumped, Rolling, Mantling and Ragdoll*/
	/** On Landed*/
	UFUNCTION(BlueprintCallable, Category = "PR|Character States")
		void EventOnLanded();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "PR|Character States")
		void Multicast_OnLanded();

	/** On Jumped*/
	UFUNCTION(BlueprintCallable, Category = "PR|Character States")
		void EventOnJumped();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "PR|Character States")
		void Multicast_OnJumped();

	/** Rolling Montage Play Replication*/
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "PR|Character States")
		void Server_PlayMontage(UAnimMontage* Montage, float PlayRate);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "PR|Character States")
		void Multicast_PlayMontage(UAnimMontage* Montage, float PlayRate);

	/** Ragdolling*/
	UFUNCTION(BlueprintCallable, Category = "PR|Character States")
		void ReplicatedRagdollStart();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "PR|Character States")
		void Server_RagdollStart();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "PR|Character States")
		void Multicast_RagdollStart();

	UFUNCTION(BlueprintCallable, Category = "PR|Character States")
		void ReplicatedRagdollEnd();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "PR|Character States")
		void Server_RagdollEnd(FVector CharacterLocation);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "PR|Character States")
		void Multicast_RagdollEnd(FVector CharacterLocation);

	/** Input */

	UPROPERTY(BlueprintAssignable, Category = "PR|Input")
		FJumpPressedSignature JumpPressedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "PR|Input")
		FOnJumpedSignature OnJumpedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "PR|Input")
		FRagdollStateChangedSignature RagdollStateChangedDelegate;

	UFUNCTION(BlueprintGetter, Category = "PR|Input")
		EPRStance GetDesiredStance() const { return DesiredStance; }

	UFUNCTION(BlueprintSetter, Category = "PR|Input")
		void SetDesiredStance(EPRStance NewStance);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "PR|Input")
		void Server_SetDesiredStance(EPRStance NewStance);

	UFUNCTION(BlueprintCallable, Category = "PR|Character States")
		void SetDesiredGait(EPRGait NewGait);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "PR|Character States")
		void Server_SetDesiredGait(EPRGait NewGait);

	UFUNCTION(BlueprintGetter, Category = "PR|Input")
		EPRRotationMode GetDesiredRotationMode() const { return DesiredRotationMode; }

	UFUNCTION(BlueprintSetter, Category = "PR|Input")
		void SetDesiredRotationMode(EPRRotationMode NewRotMode);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "PR|Character States")
		void Server_SetDesiredRotationMode(EPRRotationMode NewRotMode);

	/** Rotation System */

	UFUNCTION(BlueprintCallable, Category = "PR|Rotation System")
		void SetActorLocationAndTargetRotation(FVector NewLocation, FRotator NewRotation);

	/** Movement System */

	UFUNCTION(BlueprintGetter, Category = "PR|Movement System")
		bool HasMovementInput() const { return bHasMovementInput; }

	UFUNCTION(BlueprintCallable, Category = "PR|Movement System")
		FPRMovementSettings GetTargetMovementSettings() const;

	UFUNCTION(BlueprintCallable, Category = "PR|Movement System")
		EPRGait GetAllowedGait() const;

	UFUNCTION(BlueprintCallable, Category = "PR|Movement System")
		EPRGait GetActualGait(EPRGait AllowedGait) const;

	UFUNCTION(BlueprintCallable, Category = "PR|Movement System")
		bool CanSprint() const;

	/** BP implementable function that called when Breakfall starts */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Movement System")
		void OnBreakfall();
	virtual void OnBreakfall_Implementation();

	/** BP implementable function that called when A Montage starts, e.g. during rolling */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Movement System")
		void Replicated_PlayMontage(UAnimMontage* Montage, float PlayRate);
	virtual void Replicated_PlayMontage_Implementation(UAnimMontage* Montage, float PlayRate);

	/** Implement on BP to get required roll animation according to character's state */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "PR|Movement System")
		UAnimMontage* GetRollAnimation();

	/** Utility */

	UFUNCTION(BlueprintCallable, Category = "PR|Utility")
		float GetAnimCurveValue(FName CurveName) const;

	UFUNCTION(BlueprintCallable, Category = "PR|Utility")
		void SetVisibleMesh(USkeletalMesh* NewSkeletalMesh);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "PR|Utility")
		void Server_SetVisibleMesh(USkeletalMesh* NewSkeletalMesh);

	/** Camera System */

	UFUNCTION(BlueprintGetter, Category = "PR|Camera System")
		bool IsRightShoulder() const { return bRightShoulder; }

	UFUNCTION(BlueprintCallable, Category = "PR|Camera System")
		void SetRightShoulder(bool bNewRightShoulder);

	UFUNCTION(BlueprintCallable, Category = "PR|Camera System")
		virtual ECollisionChannel GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius);

	UFUNCTION(BlueprintCallable, Category = "PR|Camera System")
		virtual FTransform GetThirdPersonPivotTarget();

	UFUNCTION(BlueprintCallable, Category = "PR|Camera System")
		virtual FVector GetFirstPersonCameraTarget();

	UFUNCTION(BlueprintCallable, Category = "PR|Camera System")
		void GetCameraParameters(float& TPFOVOut, float& FPFOVOut, bool& bRightShoulderOut) const;

	UFUNCTION(BlueprintCallable, Category = "PR|Camera System")
		void SetCameraBehavior(UPRPlayerCameraBehavior* CamBeh) { CameraBehavior = CamBeh; }

	/** Essential Information Getters/Setters */

	UFUNCTION(BlueprintGetter, Category = "PR|Essential Information")
		FVector GetAcceleration() const { return Acceleration; }

	UFUNCTION(BlueprintGetter, Category = "PR|Essential Information")
		bool IsMoving() const { return bIsMoving; }

	UFUNCTION(BlueprintCallable, Category = "PR|Essential Information")
		FVector GetMovementInput() const;

	UFUNCTION(BlueprintGetter, Category = "PR|Essential Information")
		float GetMovementInputAmount() const { return MovementInputAmount; }

	UFUNCTION(BlueprintGetter, Category = "PR|Essential Information")
		float GetSpeed() const { return Speed; }

	UFUNCTION(BlueprintCallable, Category = "PR|Essential Information")
		FRotator GetAimingRotation() const { return AimingRotation; }

	UFUNCTION(BlueprintGetter, Category = "PR|Essential Information")
		float GetAimYawRate() const { return AimYawRate; }

	/** Input */

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Input")
		void OnForwardMovement(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Input")
		void OnRightMovement(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Input")
		void OnCameraUp(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Input")
		void OnCameraRight(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Input")
		void OnCameraRot(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Input")
		void OnJump(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Input")
		void OnSprint(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Input")
		void OnCameraTap(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Input")
		void OnCameraHeld(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Input")
		void OnStance(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Input")
		void OnWalk(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Input")
		void OnRagdoll(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Input")
		void OnVelocityDirection(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Input")
		void OnLookingDirection(const FInputActionValue& Value);

protected:
	/** Ragdoll System */

	void RagdollUpdate(float DeltaTime);

	void SetActorLocationDuringRagdoll(float DeltaTime);

	/** State Changes */

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	virtual void OnMovementStateChanged(EPRMovementState PreviousState);

	virtual void OnMovementActionChanged(EPRMovementAction PreviousAction);

	virtual void OnStanceChanged(EPRStance PreviousStance);

	virtual void OnRotationModeChanged(EPRRotationMode PreviousRotationMode);

	virtual void OnGaitChanged(EPRGait PreviousGait);

	virtual void OnViewModeChanged(EPRViewMode PreviousViewMode);

	virtual void OnOverlayStateChanged(EPROverlayState PreviousState);

	virtual void OnVisibleMeshChanged(const USkeletalMesh* PreviousSkeletalMesh);

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual void OnJumped_Implementation() override;

	virtual void Landed(const FHitResult& Hit) override;

	void OnLandFrictionReset();

	void SetEssentialValues(float DeltaTime);

	void UpdateCharacterMovement();

	void UpdateGroundedRotation(float DeltaTime);

	void UpdateInAirRotation(float DeltaTime);

	/** Utils */

	void SmoothCharacterRotation(FRotator Target, float TargetInterpSpeed, float ActorInterpSpeed, float DeltaTime);

	float CalculateGroundedRotationRate() const;

	void LimitRotation(float AimYawMin, float AimYawMax, float InterpSpeed, float DeltaTime);

	void SetMovementModel();

	void ForceUpdateCharacterState();

	/** Replication */
	UFUNCTION(Category = "PR|Replication")
		void OnRep_RotationMode(EPRRotationMode PrevRotMode);

	UFUNCTION(Category = "PR|Replication")
		void OnRep_ViewMode(EPRViewMode PrevViewMode);

	UFUNCTION(Category = "PR|Replication")
		void OnRep_OverlayState(EPROverlayState PrevOverlayState);

	UFUNCTION(Category = "PR|Replication")
		void OnRep_VisibleMesh(const USkeletalMesh* PreviousSkeletalMesh);

protected:
	/* Custom movement component*/
	UPROPERTY()
		TObjectPtr<UPRCharacterMovementComponent> MyCharacterMovementComponent;

	/** Input */

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "PR|Input")
		EPRRotationMode DesiredRotationMode = EPRRotationMode::LookingDirection;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "PR|Input")
		EPRGait DesiredGait = EPRGait::Running;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "PR|Input")
		EPRStance DesiredStance = EPRStance::Standing;

	UPROPERTY(EditDefaultsOnly, Category = "PR|Input", BlueprintReadOnly)
		float LookUpDownRate = 1.25f;

	UPROPERTY(EditDefaultsOnly, Category = "PR|Input", BlueprintReadOnly)
		float LookLeftRightRate = 1.25f;

	UPROPERTY(EditDefaultsOnly, Category = "PR|Input", BlueprintReadOnly)
		float RollDoubleTapTimeout = 0.3f;

	UPROPERTY(Category = "PR|Input", BlueprintReadOnly)
		bool bBreakFall = false;

	UPROPERTY(Category = "PR|Input", BlueprintReadOnly)
		bool bSprintHeld = false;

	/** Camera System */

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PR|Camera System")
		float ThirdPersonFOV = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PR|Camera System")
		float FirstPersonFOV = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PR|Camera System")
		bool bRightShoulder = false;

	/** Movement System */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PR|Movement System")
		FDataTableRowHandle MovementModel;

	/** Essential Information */

	UPROPERTY(BlueprintReadOnly, Category = "PR|Essential Information")
		FVector Acceleration = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "PR|Essential Information")
		bool bIsMoving = false;

	UPROPERTY(BlueprintReadOnly, Category = "PR|Essential Information")
		bool bHasMovementInput = false;

	UPROPERTY(BlueprintReadOnly, Category = "PR|Essential Information")
		FRotator LastVelocityRotation;

	UPROPERTY(BlueprintReadOnly, Category = "PR|Essential Information")
		FRotator LastMovementInputRotation;

	UPROPERTY(BlueprintReadOnly, Category = "PR|Essential Information")
		float Speed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "PR|Essential Information")
		float MovementInputAmount = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "PR|Essential Information")
		float AimYawRate = 0.0f;

	/** Replicated Essential Information*/

	UPROPERTY(BlueprintReadOnly, Category = "PR|Essential Information")
		float EasedMaxAcceleration = 0.0f;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "PR|Essential Information")
		FVector ReplicatedCurrentAcceleration = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "PR|Essential Information")
		FRotator ReplicatedControlRotation = FRotator::ZeroRotator;

	/** Replicated Skeletal Mesh Information*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Skeletal Mesh", ReplicatedUsing = OnRep_VisibleMesh)
		TObjectPtr<USkeletalMesh> VisibleMesh = nullptr;

	/** State Values */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|State Values", ReplicatedUsing = OnRep_OverlayState)
		EPROverlayState OverlayState = EPROverlayState::Default;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|State Values")
		bool bNeedToResetOverlayState = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|State Values")
		bool bIsReloading = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|State Values")
		bool bIsEquipping = false;

	UPROPERTY(BlueprintReadOnly, Category = "PR|State Values")
		EPRGroundedEntryState GroundedEntryState;

	UPROPERTY(BlueprintReadOnly, Category = "PR|State Values")
		EPRMovementState MovementState = EPRMovementState::None;

	UPROPERTY(BlueprintReadOnly, Category = "PR|State Values")
		EPRMovementState PrevMovementState = EPRMovementState::None;

	UPROPERTY(BlueprintReadOnly, Category = "PR|State Values")
		EPRMovementAction MovementAction = EPRMovementAction::None;

	UPROPERTY(BlueprintReadOnly, Category = "PR|State Values", ReplicatedUsing = OnRep_RotationMode)
		EPRRotationMode RotationMode = EPRRotationMode::LookingDirection;

	UPROPERTY(BlueprintReadOnly, Category = "PR|State Values")
		EPRGait Gait = EPRGait::Walking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PR|State Values")
		EPRStance Stance = EPRStance::Standing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PR|State Values", ReplicatedUsing = OnRep_ViewMode)
		EPRViewMode ViewMode = EPRViewMode::ThirdPerson;

	UPROPERTY(BlueprintReadOnly, Category = "PR|State Values")
		int32 OverlayOverrideState = 0;

	/** Movement System */

	UPROPERTY(BlueprintReadOnly, Category = "PR|Movement System")
		FPRMovementStateSettings MovementData;

	/** Rotation System */

	UPROPERTY(BlueprintReadOnly, Category = "PR|Rotation System")
		FRotator TargetRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadOnly, Category = "PR|Rotation System")
		FRotator InAirRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadOnly, Category = "PR|Rotation System")
		float YawOffset = 0.0f;

	/** Breakfall System */

	/** If player hits to the ground with a specified amount of velocity, switch to breakfall state */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "PR|Breakfall System")
		bool bBreakfallOnLand = true;

	/** If player hits to the ground with an amount of velocity greater than specified value, switch to breakfall state */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "PR|Breakfall System", meta = (EditCondition = "bBreakfallOnLand"))
		float BreakfallOnLandVelocity = 700.0f;

	/** Ragdoll System */

	/** If the skeleton uses a reversed pelvis bone, flip the calculation operator */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "PR|Ragdoll System")
		bool bReversedPelvis = false;

	/** If player hits to the ground with a specified amount of velocity, switch to ragdoll state */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "PR|Ragdoll System")
		bool bRagdollOnLand = false;

	/** If player hits to the ground with an amount of velocity greater than specified value, switch to ragdoll state */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "PR|Ragdoll System", meta = (EditCondition = "bRagdollOnLand"))
		float RagdollOnLandVelocity = 1000.0f;

	UPROPERTY(BlueprintReadOnly, Category = "PR|Ragdoll System")
		bool bRagdollOnGround = false;

	UPROPERTY(BlueprintReadOnly, Category = "PR|Ragdoll System")
		bool bRagdollFaceUp = false;

	UPROPERTY(BlueprintReadOnly, Category = "PR|Ragdoll System")
		FVector LastRagdollVelocity = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "PR|Ragdoll System")
		FVector TargetRagdollLocation = FVector::ZeroVector;

	/* Server ragdoll pull force storage*/
	float ServerRagdollPull = 0.0f;

	/* Dedicated server mesh default visibility based anim tick option*/
	EVisibilityBasedAnimTickOption DefVisBasedTickOp;

	bool bPreRagdollURO = false;

	/** Cached Variables */

	FVector PreviousVelocity = FVector::ZeroVector;

	float PreviousAimYaw = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "PR|Camera")
		TObjectPtr<UPRPlayerCameraBehavior> CameraBehavior;

	/** Last time the 'first' crouch/roll button is pressed */
	float LastStanceInputTime = 0.0f;

	/* Timer to manage reset of braking friction factor after on landed event */
	FTimerHandle OnLandedFrictionResetTimer;

	/* Smooth out aiming by interping control rotation*/
	FRotator AimingRotation = FRotator::ZeroRotator;

	/** We won't use curve based movement and a few other features on networked games */
	bool bEnableNetworkOptimizations = false;

private:
	UPROPERTY()
	TObjectPtr<UPRDebugComponent> PRDebugComponent = nullptr;
};

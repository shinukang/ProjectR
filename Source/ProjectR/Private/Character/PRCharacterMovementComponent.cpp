#include "Character/PRCharacterMovementComponent.h"
#include "Character/PRBaseCharacter.h"

#include "Curves/CurveVector.h"

UPRCharacterMovementComponent::UPRCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPRCharacterMovementComponent::OnMovementUpdated(float DeltaTime, const FVector& OldLocation,
                                                       const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaTime, OldLocation, OldVelocity);

	if (!CharacterOwner)
	{
		return;
	}

	// Set Movement Settings
	if (bRequestMovementSettingsChange)
	{
		const float UpdateMaxWalkSpeed = CurrentMovementSettings.GetSpeedForGait(AllowedGait);
		MaxWalkSpeed = UpdateMaxWalkSpeed;
		MaxWalkSpeedCrouched = UpdateMaxWalkSpeed;

		bRequestMovementSettingsChange = false;
	}
}

void UPRCharacterMovementComponent::PhysWalking(float deltaTime, int32 Iterations)
{
	if (CurrentMovementSettings.MovementCurve)
	{
		// Update the Ground Friction using the Movement Curve.
		// This allows for fine control over movement behavior at each speed.
		GroundFriction = CurrentMovementSettings.MovementCurve->GetVectorValue(GetMappedSpeed()).Z;
	}
	Super::PhysWalking(deltaTime, Iterations);
}

float UPRCharacterMovementComponent::GetMaxAcceleration() const
{
	// Update the Acceleration using the Movement Curve.
	// This allows for fine control over movement behavior at each speed.
	if (!IsMovingOnGround() || !CurrentMovementSettings.MovementCurve)
	{
		return Super::GetMaxAcceleration();
	}
	return CurrentMovementSettings.MovementCurve->GetVectorValue(GetMappedSpeed()).X;
}

float UPRCharacterMovementComponent::GetMaxBrakingDeceleration() const
{
	// Update the Deceleration using the Movement Curve.
	// This allows for fine control over movement behavior at each speed.
	if (!IsMovingOnGround() || !CurrentMovementSettings.MovementCurve)
	{
		return Super::GetMaxBrakingDeceleration();
	}
	return CurrentMovementSettings.MovementCurve->GetVectorValue(GetMappedSpeed()).Y;
}

void UPRCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags) // Client only
{
	Super::UpdateFromCompressedFlags(Flags);

	bRequestMovementSettingsChange = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

class FNetworkPredictionData_Client* UPRCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr);

	if (!ClientPredictionData)
	{
		UPRCharacterMovementComponent* MutableThis = const_cast<UPRCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_My(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void UPRCharacterMovementComponent::FSavedMove_My::Clear()
{
	Super::Clear();

	bSavedRequestMovementSettingsChange = false;
	SavedAllowedGait = EPRGait::Walking;
}

uint8 UPRCharacterMovementComponent::FSavedMove_My::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (bSavedRequestMovementSettingsChange)
	{
		Result |= FLAG_Custom_0;
	}

	return Result;
}

void UPRCharacterMovementComponent::FSavedMove_My::SetMoveFor(ACharacter* Character, float InDeltaTime,
                                                               FVector const& NewAccel,
                                                               class FNetworkPredictionData_Client_Character&
                                                               ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UPRCharacterMovementComponent* CharacterMovement = Cast<UPRCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		bSavedRequestMovementSettingsChange = CharacterMovement->bRequestMovementSettingsChange;
		SavedAllowedGait = CharacterMovement->AllowedGait;
	}
}

void UPRCharacterMovementComponent::FSavedMove_My::PrepMoveFor(ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UPRCharacterMovementComponent* CharacterMovement = Cast<UPRCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		CharacterMovement->AllowedGait = SavedAllowedGait;
	}
}

UPRCharacterMovementComponent::FNetworkPredictionData_Client_My::FNetworkPredictionData_Client_My(
	const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{
}

FSavedMovePtr UPRCharacterMovementComponent::FNetworkPredictionData_Client_My::AllocateNewMove()
{
	return MakeShared<FSavedMove_My>();
}

void UPRCharacterMovementComponent::Server_SetAllowedGait_Implementation(const EPRGait NewAllowedGait)
{
	AllowedGait = NewAllowedGait;
}

float UPRCharacterMovementComponent::GetMappedSpeed() const
{
	// Map the character's current speed to the configured movement speeds with a range of 0-3,
	// with 0 = stopped, 1 = the Walk Speed, 2 = the Run Speed, and 3 = the Sprint Speed.
	// This allows us to vary the movement speeds but still use the mapped range in calculations for consistent results

	const float Speed = Velocity.Size2D();
	const float LocWalkSpeed = CurrentMovementSettings.WalkSpeed;
	const float LocRunSpeed = CurrentMovementSettings.RunSpeed;
	const float LocSprintSpeed = CurrentMovementSettings.SprintSpeed;

	if (Speed > LocRunSpeed)
	{
		return FMath::GetMappedRangeValueClamped<float, float>({LocRunSpeed, LocSprintSpeed}, {2.0f, 3.0f}, Speed);
	}

	if (Speed > LocWalkSpeed)
	{
		return FMath::GetMappedRangeValueClamped<float, float>({LocWalkSpeed, LocRunSpeed}, {1.0f, 2.0f}, Speed);
	}

	return FMath::GetMappedRangeValueClamped<float, float>({0.0f, LocWalkSpeed}, {0.0f, 1.0f}, Speed);
}

void UPRCharacterMovementComponent::SetMovementSettings(FPRMovementSettings NewMovementSettings)
{
	// Set the current movement settings from the owner
	CurrentMovementSettings = NewMovementSettings;
	bRequestMovementSettingsChange = true;
}

void UPRCharacterMovementComponent::SetAllowedGait(EPRGait NewAllowedGait)
{
	if (AllowedGait != NewAllowedGait)
	{
		if (PawnOwner->IsLocallyControlled())
		{
			AllowedGait = NewAllowedGait;
			if (GetCharacterOwner()->GetLocalRole() == ROLE_AutonomousProxy)
			{
				Server_SetAllowedGait(NewAllowedGait);
			}
			bRequestMovementSettingsChange = true;
			return;
		}
		if (!PawnOwner->HasAuthority())
		{
			const float UpdateMaxWalkSpeed = CurrentMovementSettings.GetSpeedForGait(AllowedGait);
			MaxWalkSpeed = UpdateMaxWalkSpeed;
			MaxWalkSpeedCrouched = UpdateMaxWalkSpeed;
		}
	}
}

#pragma once

#include "CoreMinimal.h"
#include "PRCharacterEnumLibrary.h"

#include "PRStructEnumLibrary.generated.h"


USTRUCT(BlueprintType)
struct FPRMovementState
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Movement System")
	EPRMovementState State = EPRMovementState::None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Movement System")
	bool None_ = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Movement System")
	bool Grounded_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Movement System")
	bool InAir_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Movement System")
	bool Mantling_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Movement System")
	bool Ragdoll_ = false;

public:
	FPRMovementState()
	{
	}

	FPRMovementState(const EPRMovementState InitialState) { *this = InitialState; }

	const bool& None() const { return None_; }
	const bool& Grounded() const { return Grounded_; }
	const bool& InAir() const { return InAir_; }
	const bool& Mantling() const { return Mantling_; }
	const bool& Ragdoll() const { return Ragdoll_; }

	operator EPRMovementState() const { return State; }

	void operator=(const EPRMovementState NewState)
	{
		State = NewState;
		None_ = State == EPRMovementState::None;
		Grounded_ = State == EPRMovementState::Grounded;
		InAir_ = State == EPRMovementState::InAir;
		Mantling_ = State == EPRMovementState::Mantling;
		Ragdoll_ = State == EPRMovementState::Ragdoll;
	}
};

USTRUCT(BlueprintType)
struct FPRStance
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Character States")
	EPRStance Stance = EPRStance::Standing;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Character States")
	bool Standing_ = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Character States")
	bool Crouching_ = false;

public:
	FPRStance()
	{
	}

	FPRStance(const EPRStance InitialStance) { *this = InitialStance; }

	const bool& Standing() const { return Standing_; }
	const bool& Crouching() const { return Crouching_; }

	operator EPRStance() const { return Stance; }

	void operator=(const EPRStance NewStance)
	{
		Stance = NewStance;
		Standing_ = Stance == EPRStance::Standing;
		Crouching_ = Stance == EPRStance::Crouching;
	}
};

USTRUCT(BlueprintType)
struct FPRRotationMode
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Rotation System")
	EPRRotationMode RotationMode = EPRRotationMode::VelocityDirection;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Rotation System")
	bool VelocityDirection_ = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Rotation System")
	bool LookingDirection_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Rotation System")
	bool Aiming_ = false;

public:
	FPRRotationMode()
	{
	}

	FPRRotationMode(const EPRRotationMode InitialRotationMode) { *this = InitialRotationMode; }

	const bool& VelocityDirection() const { return VelocityDirection_; }
	const bool& LookingDirection() const { return LookingDirection_; }
	const bool& Aiming() const { return Aiming_; }

	operator EPRRotationMode() const { return RotationMode; }

	void operator=(const EPRRotationMode NewRotationMode)
	{
		RotationMode = NewRotationMode;
		VelocityDirection_ = RotationMode == EPRRotationMode::VelocityDirection;
		LookingDirection_ = RotationMode == EPRRotationMode::LookingDirection;
		Aiming_ = RotationMode == EPRRotationMode::Aiming;
	}
};

USTRUCT(BlueprintType)
struct FPRMovementDirection
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Movement System")
	EPRMovementDirection MovementDirection = EPRMovementDirection::Forward;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Movement System")
	bool Forward_ = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Movement System")
	bool Right_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Movement System")
	bool Left_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Movement System")
	bool Backward_ = false;

public:
	FPRMovementDirection()
	{
	}

	FPRMovementDirection(const EPRMovementDirection InitialMovementDirection)
	{
		*this = InitialMovementDirection;
	}

	const bool& Forward() const { return Forward_; }
	const bool& Right() const { return Right_; }
	const bool& Left() const { return Left_; }
	const bool& Backward() const { return Backward_; }

	operator EPRMovementDirection() const { return MovementDirection; }

	void operator=(const EPRMovementDirection NewMovementDirection)
	{
		MovementDirection = NewMovementDirection;
		Forward_ = MovementDirection == EPRMovementDirection::Forward;
		Right_ = MovementDirection == EPRMovementDirection::Right;
		Left_ = MovementDirection == EPRMovementDirection::Left;
		Backward_ = MovementDirection == EPRMovementDirection::Backward;
	}
};

USTRUCT(BlueprintType)
struct FPRMovementAction
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Movement System")
	EPRMovementAction Action = EPRMovementAction::None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Movement System")
	bool None_ = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Movement System")
	bool LowMantle_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Movement System")
	bool HighMantle_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Movement System")
	bool Rolling_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Movement System")
	bool GettingUp_ = false;

public:
	FPRMovementAction()
	{
	}

	FPRMovementAction(const EPRMovementAction InitialAction) { *this = InitialAction; }

	const bool& None() const { return None_; }
	const bool& LowMantle() const { return LowMantle_; }
	const bool& HighMantle() const { return HighMantle_; }
	const bool& Rolling() const { return Rolling_; }
	const bool& GettingUp() const { return GettingUp_; }

	operator EPRMovementAction() const { return Action; }

	void operator=(const EPRMovementAction NewAction)
	{
		Action = NewAction;
		None_ = Action == EPRMovementAction::None;
		LowMantle_ = Action == EPRMovementAction::LowMantle;
		HighMantle_ = Action == EPRMovementAction::HighMantle;
		Rolling_ = Action == EPRMovementAction::Rolling;
		GettingUp_ = Action == EPRMovementAction::GettingUp;
	}
};

USTRUCT(BlueprintType)
struct FPRGait
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Movement System")
	EPRGait Gait = EPRGait::Walking;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Movement System")
	bool Walking_ = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Movement System")
	bool Running_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Movement System")
	bool Sprinting_ = false;

public:
	FPRGait()
	{
	}

	FPRGait(const EPRGait InitialGait) { *this = InitialGait; }

	const bool& Walking() const { return Walking_; }
	const bool& Running() const { return Running_; }
	const bool& Sprinting() const { return Sprinting_; }

	operator EPRGait() const { return Gait; }

	void operator=(const EPRGait NewGait)
	{
		Gait = NewGait;
		Walking_ = Gait == EPRGait::Walking;
		Running_ = Gait == EPRGait::Running;
		Sprinting_ = Gait == EPRGait::Sprinting;
	}
};

USTRUCT(BlueprintType)
struct FPROverlayState
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Character States")
	EPROverlayState State = EPROverlayState::Default;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Character States")
	bool Default_ = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Character States")
	bool Masculine_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Character States")
	bool Feminine_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Character States")
	bool Injured_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Character States")
	bool HandsTied_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Character States")
	bool Rifle_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Character States")
	bool PistolOneHanded_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Character States")
	bool PistolTwoHanded_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Character States")
	bool Bow_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Character States")
	bool Torch_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Character States")
	bool Binoculars_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Character States")
	bool Box_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Character States")
	bool Barrel_ = false;

public:
	FPROverlayState()
	{
	}

	FPROverlayState(const EPROverlayState InitialState) { *this = InitialState; }

	const bool& Default() const { return Default_; }
	const bool& Masculine() const { return Masculine_; }
	const bool& Feminine() const { return Feminine_; }
	const bool& Injured() const { return Injured_; }
	const bool& HandsTied() const { return HandsTied_; }
	const bool& Rifle() const { return Rifle_; }
	const bool& PistolOneHanded() const { return PistolOneHanded_; }
	const bool& PistolTwoHanded() const { return PistolTwoHanded_; }
	const bool& Bow() const { return Bow_; }
	const bool& Torch() const { return Torch_; }
	const bool& Binoculars() const { return Binoculars_; }
	const bool& Box() const { return Box_; }
	const bool& Barrel() const { return Barrel_; }

	operator EPROverlayState() const { return State; }

	void operator=(const EPROverlayState NewAction)
	{
		State = NewAction;
		Default_ = State == EPROverlayState::Default;
		Masculine_ = State == EPROverlayState::Masculine;
		Feminine_ = State == EPROverlayState::Feminine;
		Injured_ = State == EPROverlayState::Injured;
		HandsTied_ = State == EPROverlayState::HandsTied;
		Rifle_ = State == EPROverlayState::Rifle;
		PistolOneHanded_ = State == EPROverlayState::PistolOneHanded;
		PistolTwoHanded_ = State == EPROverlayState::PistolTwoHanded;
		Bow_ = State == EPROverlayState::Bow;
		Torch_ = State == EPROverlayState::Torch;
		Binoculars_ = State == EPROverlayState::Binoculars;
		Box_ = State == EPROverlayState::Box;
		Barrel_ = State == EPROverlayState::Barrel;
	}
};

USTRUCT(BlueprintType)
struct FPRGroundedEntryState
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Breakfall System")
	EPRGroundedEntryState State = EPRGroundedEntryState::None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Breakfall System")
	bool None_ = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "PR|Breakfall System")
	bool Roll_ = false;

public:
	FPRGroundedEntryState()
	{
	}

	FPRGroundedEntryState(const EPRGroundedEntryState InitialState) { *this = InitialState; }

	const bool& None() const { return None_; }
	const bool& Roll() const { return Roll_; }

	operator EPRGroundedEntryState() const { return State; }

	void operator=(const EPRGroundedEntryState NewState)
	{
		State = NewState;
		None_ = State == EPRGroundedEntryState::None;
		Roll_ = State == EPRGroundedEntryState::Roll;
	}
};

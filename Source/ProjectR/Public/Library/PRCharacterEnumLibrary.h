#pragma once

#include "CoreMinimal.h"
#include "PRCharacterEnumLibrary.generated.h"

/* Returns the enumeration index. */
template <typename Enumeration>
static FORCEINLINE int32 GetEnumerationIndex(const Enumeration InValue)
{
	return StaticEnum<Enumeration>()->GetIndexByValue(static_cast<int64>(InValue));
}

/* Returns the enumeration value as string. */
template <typename Enumeration>
static FORCEINLINE FString GetEnumerationToString(const Enumeration InValue)
{
	return StaticEnum<Enumeration>()->GetNameStringByValue(static_cast<int64>(InValue));
}

/**
 * Character gait state. Note: Also edit related struct in PRStructEnumLibrary if you add new enums
 */
UENUM(BlueprintType)
enum class EPRGait : uint8
{
	Walking,
	Running,
	Sprinting
};

/**
 * Character movement action state. Note: Also edit related struct in PRStructEnumLibrary if you add new enums
 */
UENUM(BlueprintType)
enum class EPRMovementAction : uint8
{
	None,
	LowMantle,
	HighMantle,
	Rolling,
	GettingUp
};

/**
 * Character movement state. Note: Also edit related struct in PRStructEnumLibrary if you add new enums
 */
UENUM(BlueprintType)
enum class EPRMovementState : uint8
{
	None,
	Grounded,
	InAir,
	Mantling,
	Ragdoll
};

/**
 * Character overlay state. Note: Also edit related struct in PRStructEnumLibrary if you add new enums
 */
UENUM(BlueprintType)
enum class EPROverlayState : uint8
{
	Default,
	Masculine,
	Feminine,
	Injured,
	HandsTied,
	Rifle,
	PistolOneHanded,
	PistolTwoHanded,
	Bow,
	Torch,
	Binoculars,
	Box,
	Barrel
};

/**
 * Character rotation mode. Note: Also edit related struct in PRStructEnumLibrary if you add new enums
 */
UENUM(BlueprintType)
enum class EPRRotationMode : uint8
{
	VelocityDirection,
	LookingDirection,
	Aiming
};

/**
 * Character stance. Note: Also edit related struct in PRStructEnumLibrary if you add new enums
 */
UENUM(BlueprintType)
enum class EPRStance : uint8
{
	Standing,
	Crouching
};

/**
 * Character view mode. Note: Also edit related struct in PRStructEnumLibrary if you add new enums
 */
UENUM(BlueprintType)
enum class EPRViewMode : uint8
{
	ThirdPerson,
	FirstPerson
};

UENUM(BlueprintType)
enum class EPRAnimFeatureExample : uint8
{
	StrideBlending,
	AdditiveBlending,
	SprintImpulse
};

UENUM(BlueprintType)
enum class EPRFootstepType : uint8
{
	Step,
	WalkRun,
	Jump,
	Land
};

UENUM(BlueprintType)
enum class EPRGroundedEntryState : uint8
{
	None,
	Roll
};

UENUM(BlueprintType)
enum class EPRHipsDirection : uint8
{
	F,
	B,
	RF,
	RB,
	LF,
	LB
};

UENUM(BlueprintType)
enum class EPRMantleType : uint8
{
	HighMantle,
	LowMantle,
	FallingCatch
};

UENUM(BlueprintType)
enum class EPRMovementDirection : uint8
{
	Forward,
	Right,
	Left,
	Backward
};

UENUM(BlueprintType)
enum class EPRSpawnType : uint8
{
	Location,
	Attached
};

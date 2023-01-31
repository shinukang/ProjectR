#pragma once

#include "PRCharacterEnumLibrary.h"

#include "Kismet/KismetSystemLibrary.h"
#include "CoreMinimal.h"
#include "Library/PRCharacterStructLibrary.h"
#include "PRMathLibrary.generated.h"

class UCapsuleComponent;

/**
 * Math library functions for PR
 */
UCLASS()
class PROJECTR_API UPRMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "PR|Math Utils")
	static FTransform MantleComponentLocalToWorld(const FPRComponentAndTransform& CompAndTransform);

	UFUNCTION(BlueprintCallable, Category = "PR|Math Utils")
	static FTransform TransformSub(const FTransform& T1, const FTransform& T2)
	{
		return FTransform(T1.GetRotation().Rotator() - T2.GetRotation().Rotator(),
		                  T1.GetLocation() - T2.GetLocation(), T1.GetScale3D() - T2.GetScale3D());
	}

	UFUNCTION(BlueprintCallable, Category = "PR|Math Utils")
	static FTransform TransformAdd(const FTransform& T1, const FTransform& T2)
	{
		return FTransform(T1.GetRotation().Rotator() + T2.GetRotation().Rotator(),
		                  T1.GetLocation() + T2.GetLocation(), T1.GetScale3D() + T2.GetScale3D());
	}

	UFUNCTION(BlueprintCallable, Category = "PR|Math Utils")
	static FVector GetCapsuleBaseLocation(float ZOffset, UCapsuleComponent* Capsule);

	UFUNCTION(BlueprintCallable, Category = "PR|Math Utils")
	static FVector GetCapsuleLocationFromBase(FVector BaseLocation, float ZOffset, UCapsuleComponent* Capsule);

	UFUNCTION(BlueprintCallable, Category = "PR|Math Utils")
	static bool CapsuleHasRoomCheck(UCapsuleComponent* Capsule, FVector TargetLocation, float HeightOffset,
	                                float RadiusOffset, EDrawDebugTrace::Type DebugType = EDrawDebugTrace::Type::None, bool DrawDebugTrace = false);

	UFUNCTION(BlueprintCallable, Category = "PR|Math Utils")
	static bool AngleInRange(float Angle, float MinAngle, float MaxAngle, float Buffer, bool IncreaseBuffer);

	UFUNCTION(BlueprintCallable, Category = "PR|Math Utils")
	static EPRMovementDirection CalculateQuadrant(EPRMovementDirection Current, float FRThreshold, float FLThreshold,
	                                               float BRThreshold,
	                                               float BLThreshold, float Buffer, float Angle);
};

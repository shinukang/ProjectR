#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Materials/MaterialInterface.h"
#include "Components/Image.h"
#include "Library/PRCharacterEnumLibrary.h"

#include "PRCharacterStructLibrary.generated.h"

class UCurveVector;
class UAnimMontage;
class UAnimSequenceBase;
class UCurveFloat;
class UNiagaraSystem;
class UMaterialInterface;
class USoundBase;
class UPrimitiveComponent;

USTRUCT(BlueprintType)
struct FPRComponentAndTransform
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Character Struct Library")
	FTransform Transform;

	UPROPERTY(EditAnywhere, Category = "Character Struct Library")
	TObjectPtr<UPrimitiveComponent> Component = nullptr;
};

USTRUCT(BlueprintType)
struct FPRCameraSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Camera")
	float TargetArmLength = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FVector SocketOffset;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float LagSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float RotationLagSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	bool bDoCollisionTest = true;
};

USTRUCT(BlueprintType)
struct FPRCameraGaitSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Camera")
	FPRCameraSettings Walking;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FPRCameraSettings Running;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FPRCameraSettings Sprinting;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FPRCameraSettings Crouching;
};

USTRUCT(BlueprintType)
struct FPRCameraStateSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Camera")
	FPRCameraGaitSettings VelocityDirection;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FPRCameraGaitSettings LookingDirection;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FPRCameraGaitSettings Aiming;
};

USTRUCT(BlueprintType)
struct FPRMantleAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	TObjectPtr<UAnimMontage> AnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	TObjectPtr<UCurveVector> PositionCorrectionCurve = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	FVector StartingOffset;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float LowHeight = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float LowPlayRate = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float LowStartPosition = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float HighHeight = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float HighPlayRate = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float HighStartPosition = 0.0f;
};

USTRUCT(BlueprintType)
struct FPRMantleParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	TObjectPtr<UAnimMontage> AnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	TObjectPtr<UCurveVector> PositionCorrectionCurve = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float StartingPosition = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float PlayRate = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	FVector StartingOffset;
};

USTRUCT(BlueprintType)
struct FPRMantleTraceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float MaxLedgeHeight = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float MinLedgeHeight = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float ReachDistance = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float ForwardTraceRadius = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float DownwardTraceRadius = 0.0f;
};

USTRUCT(BlueprintType)
struct FPRMovementSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	float WalkSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	float RunSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	float SprintSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	TObjectPtr<UCurveVector> MovementCurve = nullptr;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	TObjectPtr<UCurveFloat> RotationRateCurve = nullptr;

	float GetSpeedForGait(const EPRGait Gait) const
	{
		switch (Gait)
		{
		case EPRGait::Running:
			return RunSpeed;
		case EPRGait::Sprinting:
			return SprintSpeed;
		case EPRGait::Walking:
			return WalkSpeed;
		default:
			return RunSpeed;
		}
	}
};

USTRUCT(BlueprintType)
struct FPRMovementStanceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FPRMovementSettings Standing;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FPRMovementSettings Crouching;
};

USTRUCT(BlueprintType)
struct FPRMovementStateSettings : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FPRMovementStanceSettings VelocityDirection;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FPRMovementStanceSettings LookingDirection;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FPRMovementStanceSettings Aiming;
};

USTRUCT(BlueprintType)
struct FPRRotateInPlaceAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Rotation System")
	TObjectPtr<UAnimSequenceBase> Animation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Rotation System")
	FName SlotName;

	UPROPERTY(EditAnywhere, Category = "Rotation System")
	float SlowTurnRate = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Rotation System")
	float FastTurnRate = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Rotation System")
	float SlowPlayRate = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Rotation System")
	float FastPlayRate = 1.0f;
};

USTRUCT(BlueprintType)
struct FPRHitFX : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Surface")
	TEnumAsByte<enum EPhysicalSurface> SurfaceType;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	EPRSpawnType SoundSpawnType;

	UPROPERTY(EditAnywhere, Category = "Sound", meta = (EditCondition = "SoundSpawnType == EPRSpawnType::Attached"))
	TEnumAsByte<enum EAttachLocation::Type> SoundAttachmentType;

	UPROPERTY(EditAnywhere, Category = "Sound")
	FVector SoundLocationOffset;

	UPROPERTY(EditAnywhere, Category = "Sound")
	FRotator SoundRotationOffset;

	UPROPERTY(EditAnywhere, Category = "Decal")
	TSoftObjectPtr<UMaterialInterface> DecalMaterial;

	UPROPERTY(EditAnywhere, Category = "Decal")
	EPRSpawnType DecalSpawnType;

	UPROPERTY(EditAnywhere, Category = "Decal", meta = (EditCondition = "DecalSpawnType == EPRSpawnType::Attached"))
	TEnumAsByte<enum EAttachLocation::Type> DecalAttachmentType;

	UPROPERTY(EditAnywhere, Category = "Decal")
	float DecalLifeSpan = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Decal")
	FVector DecalSize;

	UPROPERTY(EditAnywhere, Category = "Decal")
	FVector DecalLocationOffset;

	UPROPERTY(EditAnywhere, Category = "Decal")
	FRotator DecalRotationOffset;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	TSoftObjectPtr<UNiagaraSystem> NiagaraSystem;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	EPRSpawnType NiagaraSpawnType;

	UPROPERTY(EditAnywhere, Category = "Niagara", meta = (EditCondition = "NiagaraSpawnType == EPRSpawnType::Attached"))
	TEnumAsByte<enum EAttachLocation::Type> NiagaraAttachmentType;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	FVector NiagaraLocationOffset;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	FRotator NiagaraRotationOffset;
};

USTRUCT(BlueprintType)
struct FPRCostume : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DisplayName")
	FText Name = FText::FromString(TEXT("None"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Icon")
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Category")
	EPRCostume Category = EPRCostume::Default;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<USkeletalMesh> Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<USkeletalMesh> SubMesh = nullptr;

	bool operator==(const FPRCostume& Other) const
	{
		return Equals(Other);
	}

	bool Equals(const FPRCostume& Other) const
	{
		return this->Category == Other.Category;
	}
};

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Library/PRCharacterEnumLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "PRAnimNotifyFootstep.generated.h"

class UDataTable;

/**
 * Character footstep anim notify
 */
UCLASS()
class PROJECTR_API UPRAnimNotifyFootstep : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<UDataTable> HitDataTable;

	static FName NAME_Foot_R;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Socket")
	FName FootSocketName = NAME_Foot_R;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	TEnumAsByte<ETraceTypeQuery> TraceChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	float TraceLength = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	bool bSpawnDecal = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	bool bMirrorDecalX = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	bool bMirrorDecalY = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	bool bMirrorDecalZ = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	bool bSpawnSound = true;

	static FName NAME_FootstepType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	FName SoundParameterName = NAME_FootstepType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	EPRFootstepType FootstepType = EPRFootstepType::Step;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	bool bOverrideMaskCurve = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	float VolumeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	float PitchMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	bool bSpawnNiagara = false;
};

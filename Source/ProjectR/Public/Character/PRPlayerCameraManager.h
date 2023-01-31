#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "PRPlayerCameraManager.generated.h"

// forward declarations
class UPRDebugComponent;
class APRBaseCharacter;

/**
 * Player camera manager class
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTR_API APRPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	APRPlayerCameraManager();

	UFUNCTION(BlueprintCallable, Category = "PR|Camera")
	void OnPossess(APRBaseCharacter* NewCharacter);

	UFUNCTION(BlueprintCallable, Category = "PR|Camera")
	float GetCameraBehaviorParam(FName CurveName) const;

	/** Implemented debug logic in BP */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "PR|Camera")
	void DrawDebugTargets(FVector PivotTargetLocation);

protected:
	virtual void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "PR|Camera")
	static FVector CalculateAxisIndependentLag(
		FVector CurrentLocation, FVector TargetLocation, FRotator CameraRotation, FVector LagSpeeds, float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "PR|Camera")
	bool CustomCameraBehavior(float DeltaTime, FVector& Location, FRotator& Rotation, float& FOV);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PR|Camera")
	TObjectPtr<APRBaseCharacter> ControlledCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PR|Camera")
	TObjectPtr<USkeletalMeshComponent> CameraBehavior = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PR|Camera")
	FVector RootLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PR|Camera")
	FTransform SmoothedPivotTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PR|Camera")
	FVector PivotLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PR|Camera")
	FVector TargetCameraLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PR|Camera")
	FRotator TargetCameraRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PR|Camera")
	FRotator DebugViewRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PR|Camera")
	FVector DebugViewOffset;

private:
	UPROPERTY()
	TObjectPtr<UPRDebugComponent> PRDebugComponent = nullptr;
};

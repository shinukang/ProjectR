#pragma once

#include "CoreMinimal.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/ActorComponent.h"
#include "PRDebugComponent.generated.h"

class APRBaseCharacter;
class USkeletalMesh;

UCLASS(Blueprintable, BlueprintType)
class PROJECTR_API UPRDebugComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPRDebugComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PR|Debug")
	void OnPlayerControllerInitialized(APlayerController* Controller);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	/** Implemented on BP to update layering colors */
	UFUNCTION(BlueprintImplementableEvent, Category = "PR|Debug")
	void UpdateColoringSystem();

	/** Implement on BP to draw debug spheres */
	UFUNCTION(BlueprintImplementableEvent, Category = "PR|Debug")
	void DrawDebugSpheres();

	/** Implemented on BP to set/reset layering colors */
	UFUNCTION(BlueprintImplementableEvent, Category = "PR|Debug")
	void SetResetColors();

	/** Implemented on BP to set dynamic color materials for debugging */
	UFUNCTION(BlueprintImplementableEvent, Category = "PR|Debug")
	void SetDynamicMaterials();

	UFUNCTION(BlueprintCallable, Category = "PR|Debug")
	void ToggleGlobalTimeDilationLocal(float TimeDilation);

	UFUNCTION(BlueprintCallable, Category = "PR|Debug")
	void ToggleSlomo();

	UFUNCTION(BlueprintCallable, Category = "PR|Debug")
	void ToggleHud() { bShowHud = !bShowHud; }

	UFUNCTION(BlueprintCallable, Category = "PR|Debug")
	void ToggleDebugView();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Debug")
	void OpenOverlayMenu(bool bValue);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Debug")
	void OverlayMenuCycle(bool bValue);

	UFUNCTION(BlueprintCallable, Category = "PR|Debug")
	void ToggleDebugMesh();

	UFUNCTION(BlueprintCallable, Category = "PR|Debug")
	void ToggleTraces() { bShowTraces = !bShowTraces; }

	UFUNCTION(BlueprintCallable, Category = "PR|Debug")
	void ToggleDebugShapes() { bShowDebugShapes = !bShowDebugShapes; }

	UFUNCTION(BlueprintCallable, Category = "PR|Debug")
	void ToggleLayerColors() { bShowLayerColors = !bShowLayerColors; }

	UFUNCTION(BlueprintCallable, Category = "PR|Debug")
	void ToggleCharacterInfo() { bShowCharacterInfo = !bShowCharacterInfo; }

	UFUNCTION(BlueprintCallable, Category = "PR|Debug")
	bool GetDebugView() { return bDebugView; }

	UFUNCTION(BlueprintCallable, Category = "PR|Debug")
	bool GetShowTraces() { return bShowTraces; }

	UFUNCTION(BlueprintCallable, Category = "PR|Debug")
	bool GetShowDebugShapes() { return bShowDebugShapes; }

	UFUNCTION(BlueprintCallable, Category = "PR|Debug")
	bool GetShowLayerColors() { return bShowLayerColors; }

	UFUNCTION(BlueprintCallable, Category = "PR|Debug")
	void FocusedDebugCharacterCycle(bool bValue);

	// utility functions to draw trace debug shapes,
	// which are derived from Engine/Private/KismetTraceUtils.h.
	// Sadly the functions are private, which was the reason
	// why there reimplemented here.
	static void DrawDebugLineTraceSingle(const UWorld* World,
	                                     const FVector& Start,
	                                     const FVector& End,
	                                     EDrawDebugTrace::Type DrawDebugType,
	                                     bool bHit,
	                                     const FHitResult& OutHit,
	                                     FLinearColor TraceColor,
	                                     FLinearColor TraceHitColor,
	                                     float DrawTime);

	static void DrawDebugCapsuleTraceSingle(const UWorld* World,
	                                        const FVector& Start,
	                                        const FVector& End,
	                                        const FCollisionShape& CollisionShape,
	                                        EDrawDebugTrace::Type DrawDebugType,
	                                        bool bHit,
	                                        const FHitResult& OutHit,
	                                        FLinearColor TraceColor,
	                                        FLinearColor TraceHitColor,
	                                        float DrawTime);

	static void DrawDebugSphereTraceSingle(const UWorld* World,
	                                       const FVector& Start,
	                                       const FVector& End,
	                                       const FCollisionShape& CollisionShape,
	                                       EDrawDebugTrace::Type DrawDebugType,
	                                       bool bHit,
	                                       const FHitResult& OutHit,
	                                       FLinearColor TraceColor,
	                                       FLinearColor TraceHitColor,
	                                       float DrawTime);

protected:
	void DetectDebuggableCharactersInWorld();

public:
	UPROPERTY(BlueprintReadOnly, Category = "PR|Debug")
	TObjectPtr<APRBaseCharacter> OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Debug")
	bool bSlomo = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Debug")
	bool bShowHud = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Debug")
	bool bShowCharacterInfo = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PR|Debug")
	TObjectPtr<USkeletalMesh> DebugSkeletalMesh = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "PR|Debug")
	TArray<TObjectPtr<APRBaseCharacter>> AvailableDebugCharacters;

	UPROPERTY(BlueprintReadOnly, Category = "PR|Debug")
	TObjectPtr<APRBaseCharacter> DebugFocusCharacter = nullptr;
private:
	static bool bDebugView;

	static bool bShowTraces;

	static bool bShowDebugShapes;

	static bool bShowLayerColors;

	bool bNeedsColorReset = false;

	bool bDebugMeshVisible = false;

	UPROPERTY()
	TObjectPtr<USkeletalMesh> DefaultSkeletalMesh = nullptr;

	/// Stores the index, which is used to select the next focused debug PRBaseCharacter.
	/// If no characters where found during BeginPlay the value should be set to INDEX_NONE.
	int32 FocusedDebugCharacterIndex = INDEX_NONE;
};

#pragma once

#include "CoreMinimal.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/ActorComponent.h"
#include "InputMappingContext.h"
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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Debug")
	void OpenOverlayMenu(bool bValue);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Debug")
	void OverlayMenuCycle(bool bValue);

	UFUNCTION(BlueprintCallable, Category = "PR|Debug")
	bool GetDebugView() { return bDebugView; }

	UFUNCTION(BlueprintCallable, Category = "PR|Debug")
	bool GetShowTraces() { return bShowTraces; }
	
	UFUNCTION(BlueprintCallable, Category = "PR|Debug")
	bool GetShowDebugShapes() { return bShowDebugShapes; }

	UFUNCTION(BlueprintCallable, Category = "PR|Debug")
	bool GetShowLayerColors() { return bShowLayerColors; }

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

private:
	UFUNCTION()
	void IA_ToggleHud(const FInputActionValue& Value) { bShowHud = !bShowHud; }

	UFUNCTION()
	void IA_ToggleTraces(const FInputActionValue& Value) { bShowTraces = !bShowTraces; }

	UFUNCTION()
	void IA_ToggleLayerColors(const FInputActionValue& Value) { bShowLayerColors = !bShowLayerColors; }

	UFUNCTION()
	void IA_ToggleShapes(const FInputActionValue& Value) { bShowDebugShapes = !bShowDebugShapes; }

	UFUNCTION()
	void IA_ToggleCharacterInfo(const FInputActionValue& Value) { bShowCharacterInfo = !bShowCharacterInfo; }

	UFUNCTION()
	void IA_ToggleDebugView(const FInputActionValue& Value);

	UFUNCTION()
	void IA_FocusedCharacterCycleAction(const FInputActionValue& Value);

	UFUNCTION()
	void IA_OpenOverlayMenu(const FInputActionValue& Value);

	UFUNCTION()
	void IA_OverlayMenuCycle(const FInputActionValue& Value);

	UFUNCTION()
	void IA_ToggleDebugMesh(const FInputActionValue& Value);

	UFUNCTION()
	void IA_ToggleSlomo(const FInputActionValue& Value);

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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PR|Input")
	TObjectPtr<UInputMappingContext> DebugInputMappingContext = nullptr;

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

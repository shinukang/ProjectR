#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "Interface/PRInteractInterface.h"
#include "Widget/PRWidgetBase.h"
#include "PRPlayerController.generated.h"

class APRBaseCharacter;
class UInputMappingContext;

/**
 * Player controller class
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTR_API APRPlayerController : public APlayerController, public IPRInteractInterface, public IPRWidgetInterface
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* NewPawn) override;

	virtual void OnRep_Pawn() override;

	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable)
	void SetInputMode(EInputMode InputMode);

protected:
	void SetupCamera();

	/** Inputs On Character */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	void IA_ForwardMovement(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	void IA_RightMovement(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	void IA_CameraUp(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	void IA_CameraRight(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	void IA_Jump(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	void IA_Sprint(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	void IA_Aim(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	void IA_CameraTap(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	void IA_CameraHeld(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	void IA_Stance(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	void IA_Walk(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	void IA_Ragdoll(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	void IA_VelocityDirection(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	void IA_LookingDirection(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	void IA_Interact(const FInputActionValue& Value);

public:
	/** Main character reference */
	UPROPERTY(BlueprintReadOnly, Category = "PR")
	TObjectPtr<APRBaseCharacter> PossessedCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> RestrictedInputMappingContext = nullptr;

private:
	TObjectPtr<UPRInteractComponent> PRInteractComponent = nullptr;

	TObjectPtr<UPRInventoryComponent> PRInventoryComponent = nullptr;
};

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
	virtual void OnPossess(APawn* NewPawn) override;

	virtual void OnRep_Pawn() override;

	virtual void SetupInputComponent() override;

protected:
	void SetupCamera();

public:
	/** Main character reference */
	UPROPERTY(BlueprintReadOnly, Category = "PR")
	TObjectPtr<APRBaseCharacter> PossessedCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PR|Input")
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext = nullptr;
};

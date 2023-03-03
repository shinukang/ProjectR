#include "Character/PRPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"
#include "Character/PRCharacter.h"
#include "Character/PRPlayerCameraManager.h"
#include "Component/PRDebugComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Library/RyanLibrary.h"

void APRPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);
	PossessedCharacter = Cast<APRBaseCharacter>(NewPawn);
	if (!IsRunningDedicatedServer())
	{
		// Servers want to setup camera only in listen servers.
		SetupCamera();
	}

	URyanLibrary::SetupInputs(PossessedCharacter, this, DefaultInputMappingContext, true);

	if (!IsValid(PossessedCharacter)) return;
	
	UPRDebugComponent* DebugComp = Cast<UPRDebugComponent>(PossessedCharacter->GetComponentByClass(UPRDebugComponent::StaticClass()));
	if (DebugComp)
	{
		DebugComp->OnPlayerControllerInitialized(this);
	}
}

void APRPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	PossessedCharacter = Cast<APRBaseCharacter>(GetPawn());
	SetupCamera();
	
	URyanLibrary::SetupInputs(PossessedCharacter, this, DefaultInputMappingContext, true);
	
	if (!PossessedCharacter) return;

	UPRDebugComponent* DebugComp = Cast<UPRDebugComponent>(PossessedCharacter->GetComponentByClass(UPRDebugComponent::StaticClass()));
	if (DebugComp)
	{
		DebugComp->OnPlayerControllerInitialized(this);
	}
}

void APRPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void APRPlayerController::SetupCamera()
{
	// Call "OnPossess" in Player Camera Manager when possessing a pawn
	APRPlayerCameraManager* CastedMgr = Cast<APRPlayerCameraManager>(PlayerCameraManager);
	if (PossessedCharacter && CastedMgr)
	{
		CastedMgr->OnPossess(PossessedCharacter);
	}
}

void APRPlayerController::SetInputMode(EInputMode InputMode)
{
	
}


#include "Character/PRPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"
#include "Character/PRCharacter.h"
#include "Character/PRPlayerCameraManager.h"
#include "Components/PRDebugComponent.h"
#include "Kismet/GameplayStatics.h"

void APRPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);
	PossessedCharacter = Cast<APRBaseCharacter>(NewPawn);
	if (!IsRunningDedicatedServer())
	{
		// Servers want to setup camera only in listen servers.
		SetupCamera();
	}

	SetupInputs();

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
	SetupInputs();
	
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

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->ClearActionEventBindings();
		EnhancedInputComponent->ClearActionValueBindings();
		EnhancedInputComponent->ClearDebugKeyBindings();

		BindActions(DefaultInputMappingContext);
		BindActions(DebugInputMappingContext);
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("PR Community requires Enhanced Input System to be activated in project settings to function properly"));
	}
}

void APRPlayerController::BindActions(UInputMappingContext* Context)
{
	if (Context)
	{
		const TArray<FEnhancedActionKeyMapping>& Mappings = Context->GetMappings();
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
		if (EnhancedInputComponent)
		{
			// There may be more than one keymapping assigned to one action. So, first filter duplicate action entries to prevent multiple delegate bindings
			TSet<const UInputAction*> UniqueActions;
			for (const FEnhancedActionKeyMapping& Keymapping : Mappings)
			{
				UniqueActions.Add(Keymapping.Action);
			}
			for (const UInputAction* UniqueAction : UniqueActions)
			{
				EnhancedInputComponent->BindAction(UniqueAction, ETriggerEvent::Triggered, Cast<UObject>(this), UniqueAction->GetFName());
			}
		}
	}
}

void APRPlayerController::SetupInputs()
{
	if (PossessedCharacter)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			FModifyContextOptions Options;
			Options.bForceImmediately = 1;
			Subsystem->AddMappingContext(DefaultInputMappingContext, 1, Options);
			UPRDebugComponent* DebugComp = Cast<UPRDebugComponent>(PossessedCharacter->GetComponentByClass(UPRDebugComponent::StaticClass()));
			if (DebugComp)
			{
				// Do only if we have debug component
				Subsystem->AddMappingContext(DebugInputMappingContext, 0, Options);
			}
		}
	}
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

void APRPlayerController::ForwardMovementAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->ForwardMovementAction(Value.GetMagnitude());
	}
}

void APRPlayerController::RightMovementAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->RightMovementAction(Value.GetMagnitude());
	}
}

void APRPlayerController::CameraUpAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->CameraUpAction(Value.GetMagnitude());
	}
}

void APRPlayerController::CameraRightAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->CameraRightAction(Value.GetMagnitude());
	}
}

void APRPlayerController::JumpAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->JumpAction(Value.Get<bool>());
	}
}

void APRPlayerController::SprintAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->SprintAction(Value.Get<bool>());
	}
}

void APRPlayerController::AimAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->AimAction(Value.Get<bool>());
	}
}

void APRPlayerController::CameraTapAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->CameraTapAction();
	}
}

void APRPlayerController::CameraHeldAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->CameraHeldAction();
	}
}

void APRPlayerController::StanceAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		PossessedCharacter->StanceAction();
	}
}

void APRPlayerController::WalkAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		PossessedCharacter->WalkAction();
	}
}

void APRPlayerController::RagdollAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		PossessedCharacter->RagdollAction();
	}
}

void APRPlayerController::VelocityDirectionAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		PossessedCharacter->VelocityDirectionAction();
	}
}

void APRPlayerController::LookingDirectionAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		PossessedCharacter->LookingDirectionAction();
	}
}

void APRPlayerController::DebugToggleHudAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UPRDebugComponent* DebugComp = Cast<UPRDebugComponent>(PossessedCharacter->GetComponentByClass(UPRDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleHud();
		}
	}
}

void APRPlayerController::DebugToggleDebugViewAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UPRDebugComponent* DebugComp = Cast<UPRDebugComponent>(PossessedCharacter->GetComponentByClass(UPRDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleDebugView();
		}
	}
}

void APRPlayerController::DebugToggleTracesAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UPRDebugComponent* DebugComp = Cast<UPRDebugComponent>(PossessedCharacter->GetComponentByClass(UPRDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleTraces();
		}
	}
}

void APRPlayerController::DebugToggleShapesAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UPRDebugComponent* DebugComp = Cast<UPRDebugComponent>(PossessedCharacter->GetComponentByClass(UPRDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleDebugShapes();
		}
	}
}

void APRPlayerController::DebugToggleLayerColorsAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UPRDebugComponent* DebugComp = Cast<UPRDebugComponent>(PossessedCharacter->GetComponentByClass(UPRDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleLayerColors();
		}
	}
}

void APRPlayerController::DebugToggleCharacterInfoAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UPRDebugComponent* DebugComp = Cast<UPRDebugComponent>(PossessedCharacter->GetComponentByClass(UPRDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleCharacterInfo();
		}
	}
}

void APRPlayerController::DebugToggleSlomoAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UPRDebugComponent* DebugComp = Cast<UPRDebugComponent>(PossessedCharacter->GetComponentByClass(UPRDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleSlomo();
		}
	}
}

void APRPlayerController::DebugFocusedCharacterCycleAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		UPRDebugComponent* DebugComp = Cast<UPRDebugComponent>(PossessedCharacter->GetComponentByClass(UPRDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->FocusedDebugCharacterCycle(Value.GetMagnitude() > 0);
		}
	}
}

void APRPlayerController::DebugToggleMeshAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UPRDebugComponent* DebugComp = Cast<UPRDebugComponent>(PossessedCharacter->GetComponentByClass(UPRDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleDebugMesh();
		}
	}
}

void APRPlayerController::DebugOpenOverlayMenuAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		UPRDebugComponent* DebugComp = Cast<UPRDebugComponent>(PossessedCharacter->GetComponentByClass(UPRDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->OpenOverlayMenu(Value.Get<bool>());
		}
	}
}

void APRPlayerController::DebugOverlayMenuCycleAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		UPRDebugComponent* DebugComp = Cast<UPRDebugComponent>(PossessedCharacter->GetComponentByClass(UPRDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->OverlayMenuCycle(Value.GetMagnitude() > 0);
		}
	}
}

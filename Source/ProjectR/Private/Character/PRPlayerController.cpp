#include "Character/PRPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"
#include "Character/PRCharacter.h"
#include "Character/PRPlayerCameraManager.h"
#include "Component/PRDebugComponent.h"
#include "Component/PRInteractComponent.h"
#include "Component/PRInventoryComponent.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Library/RyanLibrary.h"

void APRPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void APRPlayerController::Init()
{
	URyanLibrary::SetupInputs(this, this, DefaultInputMappingContext, true);

	SetupWidget();

	if (UPRInteractComponent* InteractComponent = Cast<UPRInteractComponent>(GetComponentByClass(UPRInteractComponent::StaticClass())))
	{
		PRInteractComponent = InteractComponent;
		this->OnPlayerControllerInitialized.AddUObject(PRInteractComponent, &UPRInteractComponent::OnPlayerControllerInitialized);
	}

	if (UPRInventoryComponent* InventoryComponent = Cast<UPRInventoryComponent>(GetComponentByClass(UPRInventoryComponent::StaticClass())))
	{
		PRInventoryComponent = InventoryComponent;
		this->OnPlayerControllerInitialized.AddUObject(PRInventoryComponent, &UPRInventoryComponent::OnPlayerControllerInitialized);
	}

	if (UPRDebugComponent* DebugComponent = Cast<UPRDebugComponent>(GetPawn()->GetComponentByClass(UPRDebugComponent::StaticClass())))
	{
		this->OnPlayerControllerInitialized.AddUObject(DebugComponent, &UPRDebugComponent::OnPlayerControllerInitialized);
	}

	OnPlayerControllerInitialized.Broadcast(this);
}


void APRPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);
	PossessedCharacter = Cast<APRBaseCharacter>(NewPawn);
	if (!IsRunningDedicatedServer())
	{
		// Servers want to setup camera only in listen servers.
		SetupCamera();
	}

	if (PossessedCharacter)
	{
		Init();
	}
}

void APRPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	PossessedCharacter = Cast<APRBaseCharacter>(GetPawn());

	SetupCamera();

	if(PossessedCharacter)
	{
		Init();
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

UPRWidgetBase* APRPlayerController::GetHUD_Implementation()
{
	return HUD.Get();
}

void APRPlayerController::IA_ForwardMovement_Implementation(const FInputActionValue& Value)
{
	PossessedCharacter->OnForwardMovement(Value);
}

void APRPlayerController::IA_RightMovement_Implementation(const FInputActionValue& Value)
{
	PossessedCharacter->OnRightMovement(Value);
}

void APRPlayerController::IA_CameraUp_Implementation(const FInputActionValue& Value)
{
	PossessedCharacter->OnCameraUp(Value);
}

void APRPlayerController::IA_CameraRight_Implementation(const FInputActionValue& Value)
{
	PossessedCharacter->OnCameraRight(Value);
}

void APRPlayerController::IA_Jump_Implementation(const FInputActionValue& Value)
{
	PossessedCharacter->OnJump(Value);
}

void APRPlayerController::IA_Sprint_Implementation(const FInputActionValue& Value)
{
	PossessedCharacter->OnSprint(Value);
}

void APRPlayerController::IA_Aim_Implementation(const FInputActionValue& Value)
{
	PossessedCharacter->OnAim(Value);
}

void APRPlayerController::IA_CameraTap_Implementation(const FInputActionValue& Value)
{
	PossessedCharacter->OnCameraTap(Value);
}

void APRPlayerController::IA_CameraHeld_Implementation(const FInputActionValue& Value)
{
	PossessedCharacter->OnCameraHeld(Value);
}

void APRPlayerController::IA_Stance_Implementation(const FInputActionValue& Value)
{
	PossessedCharacter->OnStance(Value);
}

void APRPlayerController::IA_Walk_Implementation(const FInputActionValue& Value)
{
	PossessedCharacter->OnWalk(Value);
}

void APRPlayerController::IA_Ragdoll_Implementation(const FInputActionValue& Value)
{
	PossessedCharacter->OnRagdoll(Value);
}

void APRPlayerController::IA_VelocityDirection_Implementation(const FInputActionValue& Value)
{
	PossessedCharacter->OnVelocityDirection(Value);
}

void APRPlayerController::IA_LookingDirection_Implementation(const FInputActionValue& Value)
{
	PossessedCharacter->OnLookingDirection(Value);
}

void APRPlayerController::IA_Interact_Implementation(const FInputActionValue& Value)
{
	if(PRInteractComponent && PRInventoryComponent)
	{
		const FName DetectedObjectID = IPRInteractInterface::Execute_GetObjectID(PRInteractComponent->DetectedActor);
		const int32 DetectedObjectAmount = IPRInteractInterface::Execute_GetObjectAmount(PRInteractComponent->DetectedActor);

		PRInventoryComponent->TryAddToInventory(DetectedObjectID, DetectedObjectAmount);
	}
}

void APRPlayerController::IA_Inventory_Implementation(const FInputActionValue& Value)
{
	const bool InWidget = Value.Get<bool>();

	if(InWidget)
	{
		URyanLibrary::SetupInputs(this, this, RestrictedInputMappingContext, true);
	}
	else
	{
		URyanLibrary::SetupInputs(this, this, DefaultInputMappingContext, true);
	}

	HUD->SetInventoryVisibility(InWidget);
}

void APRPlayerController::IA_Cursor_Implementation(const FInputActionValue& Value)
{
	FVector2D MousePosition, ViewportSize;
	GetMousePosition(MousePosition.X, MousePosition.Y);
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	const FVector StartPosition(MousePosition.X, MousePosition.Y, 0.0f);
	const FVector TargetPosition(ViewportSize.X  / 2.0f, ViewportSize.Y / 2.0f, 0.0f);

	HUD->UpdateSelectedSlot(180.0f - UKismetMathLibrary::FindLookAtRotation(StartPosition, TargetPosition).Yaw);
}

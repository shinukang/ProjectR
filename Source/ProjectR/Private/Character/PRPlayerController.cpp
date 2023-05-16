#include "Character/PRPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"
#include "Character/PRCharacter.h"
#include "Character/PRPlayerCameraManager.h"
#include "Component/PRDebugComponent.h"
#include "Item/PRFirearm.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Library/RyanLibrary.h"
#include "System/PRLobbyPawn.h"

void APRPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputModeGameOnly();
}

void APRPlayerController::Init()
{
	URyanLibrary::SetupInputs(this, this, DefaultInputMappingContext, true);

	SetupWidget();

	if (UPRInventoryComponent* InventoryComponent = Cast<UPRInventoryComponent>(GetPawn()->GetComponentByClass(UPRInventoryComponent::StaticClass())))
	{
		OnControllerInitialized.AddUObject(InventoryComponent, &UPRInventoryComponent::OnControllerInitialized);

		InventoryComponent->OnUpdateInteractInfo.BindUObject(HUD, &UPRWidgetBase::UpdateInteractInfo);
		InventoryComponent->OnUpdateEquipment.AddUObject(HUD, &UPRWidgetBase::UpdateEquipment);
		InventoryComponent->OnUpdateFirearm.AddUObject(HUD, &UPRWidgetBase::UpdateFirearm);
		InventoryComponent->OnUpdateGroundItems.AddUObject(HUD, &UPRWidgetBase::UpdateGroundItems);
		InventoryComponent->OnUpdateInventoryItems.AddUObject(HUD, &UPRWidgetBase::UpdateInventoryItems);

		if (APRLobbyPawn* LobbyPawn = Cast<APRLobbyPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APRLobbyPawn::StaticClass())))
		{
			InventoryComponent->OnUpdateEquipment.AddUObject(LobbyPawn, &APRLobbyPawn::UpdateEquipment);
		}
	}

	if (UPRStatusComponent* StatusComponent = Cast<UPRStatusComponent>(GetPawn()->GetComponentByClass(UPRStatusComponent::StaticClass())))
	{
		OnControllerInitialized.AddUObject(StatusComponent, &UPRStatusComponent::OnControllerInitialized);

		StatusComponent->OnUpdateHealthPoint.BindUObject(HUD, &UPRWidgetBase::UpdateHealthPointBar);
		StatusComponent->OnUpdateStamina.BindUObject(HUD, &UPRWidgetBase::UpdateStaminaBar);
	}

	if (UPRDebugComponent* DebugComponent = Cast<UPRDebugComponent>(GetPawn()->GetComponentByClass(UPRDebugComponent::StaticClass())))
	{
		OnControllerInitialized.AddUObject(DebugComponent, &UPRDebugComponent::OnPlayerControllerInitialized);
	}

	OnControllerInitialized.Broadcast(this);
}


void APRPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);
	PossessedCharacter = Cast<APRCharacter>(NewPawn);
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

	PossessedCharacter = Cast<APRCharacter>(GetPawn());

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

void APRPlayerController::SetInputModeGameOnly()
{
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(true);
	SetInputMode(InputMode);
}

void APRPlayerController::SetInputModeGameAndUI()
{
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(true);
	SetInputMode(InputMode);
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

void APRPlayerController::IA_CameraRot_Implementation(const FInputActionValue& Value)
{
	PossessedCharacter->OnCameraRot(Value);
}

void APRPlayerController::IA_Shoot_Implementation(const FInputActionValue& Value)
{
	PossessedCharacter->OnShoot(Value);
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
	PossessedCharacter->PRInventoryComponent->OnInteractAction();
}

void APRPlayerController::IA_Inventory_Implementation(const FInputActionValue& Value)
{
	const bool InWidget = Value.Get<bool>();
	HUD->SetInventoryVisibility(InWidget);
	PossessedCharacter->PRInventoryComponent->bIsInventoryOpen = InWidget;
}

void APRPlayerController::IA_Equip_Implementation(const FInputActionValue& Value)
{
	const int32 FirearmIndex = Value.Get<float>() - 1;
	PossessedCharacter->Server_HoldFirearm(FirearmIndex);
}

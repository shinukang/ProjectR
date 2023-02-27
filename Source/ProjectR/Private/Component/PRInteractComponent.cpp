// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PRInteractComponent.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraModifier_CameraShake.h"
#include "GameFramework/Character.h"
#include "Interface/PRInteractInterface.h"
#include "Interface/PRWidgetInterface.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Library/PRObjectStructLibrary.h"
#include <EnhancedInputComponent.h>
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "Library/RyanLibrary.h"

// Sets default values for this component's properties
UPRInteractComponent::UPRInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPRInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* OwnerController = Cast<APlayerController>(GetOwner());

	if (!OwnerController) // If OwnerController is null, Print Log
	{
		UE_LOG(LogTemp, Warning, TEXT("[UPRInteractComponent::InitComp] : OwnerController is null"));
	}

	// Set OwnerCam
	OwnerCam = OwnerController->PlayerCameraManager;

	SetupWidget();

	URyanLibrary::SetupInputs(this, OwnerController, InteractInputMappingContext);
}

// Called every frame
void UPRInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(AActor* NewDetectedActor = CheckInteractiveActor())
	{
		if(DetectedActor != NewDetectedActor)
		{
			// UI update
			DetectedActor = NewDetectedActor;
			const FString DetectedActorName = IPRInteractInterface::Execute_GetName(DetectedActor);
			IPRWidgetInterface::Execute_UpdateInteractInfo(HUD, DetectedActorName);
		}
	}
	else
	{
		// UI update (remove)
		if(DetectedActor != nullptr)
		{
			DetectedActor = nullptr;
			IPRWidgetInterface::Execute_UpdateInteractInfo(HUD, FString());
		}
	}
}

AActor* UPRInteractComponent::CheckInteractiveActor()
{
	if (OwnerCam)
	{
		FVector CamLoc; // Player Camera Location
		FRotator CamRot; // Player Camera Rotation
		FHitResult HitResult;

		OwnerCam->GetCameraViewPoint(CamLoc, CamRot);

		GetWorld()->LineTraceSingleByChannel(HitResult, CamLoc, CamLoc + 1000.0f * CamRot.Vector(), ECollisionChannel::ECC_Visibility);

		if (HitResult.bBlockingHit)
		{
			if (AActor* HitActor = HitResult.GetActor(); HitActor->Implements<UPRInteractInterface>() && IPRInteractInterface::Execute_CanInteract(HitActor))
			{
				return HitActor;
			}
		}
	}
	return nullptr;
}

void UPRInteractComponent::IA_Interact(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("IA_Interact"));
}


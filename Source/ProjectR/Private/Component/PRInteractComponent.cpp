// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PRInteractComponent.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraModifier_CameraShake.h"
#include "GameFramework/Character.h"
#include "Interface/PRInteractInterface.h"
#include "Interface/PRWidgetInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Library/PRItemStructLibrary.h"
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
}

// Called every frame
void UPRInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	int32 ItemAmount = -1;

	if(AActor* NewDetectedActor = CheckInteractiveActor())
	{
		if(DetectedActor != NewDetectedActor)
		{
			// UI update
			DetectedActor = NewDetectedActor;

			if(const UDataTable* ItemDataTable = URyanLibrary::GetDataTable())
			{
				const FName DetectedItemID = IPRInteractInterface::Execute_GetItemID(DetectedActor);

				ItemAmount = IPRInteractInterface::Execute_GetItemAmount(DetectedActor);

				FPRItemData* ItemData = ItemDataTable->FindRow<FPRItemData>(DetectedItemID, TEXT("ItemID[%s] isn't exist in datatable"));

				HUD->UpdateInteractInfo(ItemData, ItemAmount);
			}
		}
	}
	else
	{
		// UI update (remove)
		if(DetectedActor != nullptr)
		{
			DetectedActor = nullptr;
			HUD->UpdateInteractInfo(FPRItemData(), ItemAmount);
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

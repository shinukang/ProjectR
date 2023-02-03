// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PRInteractComponent.h"

#include "Camera/CameraComponent.h"
#include "Camera/CameraModifier_CameraShake.h"
#include "GameFramework/Character.h"
#include "Interface/PRInteractInterface.h"

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

	InitComp();
}

// Called every frame
void UPRInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (OwnerCam)
	{	
		FVector CamLoc; // Player Camera Location
		FRotator CamRot; // Player Camera Rotation
		FHitResult HitResult;

		OwnerCam->GetCameraViewPoint(CamLoc, CamRot);

		GetWorld()->LineTraceSingleByChannel(HitResult, CamLoc, CamLoc + 1000.0f * CamRot.Vector(), ECollisionChannel::ECC_Visibility);
		
		if(HitResult.bBlockingHit)
		{
			if(AActor* HitActor = HitResult.GetActor(); HitActor->Implements<UPRInteractInterface>())
			{
				if(IPRInteractInterface::Execute_CanInteract(HitActor))
				{
					const FName HitActorID = IPRInteractInterface::Execute_GetID(HitActor);

					UE_LOG(LogTemp, Warning, TEXT("HitActorID = %s"), *HitActorID.ToString());
				}
			}
		}
	}
}

void UPRInteractComponent::InitComp()
{
	const APlayerController* OwnerController = Cast<APlayerController>(GetOwner());

	if (!OwnerController) // If OwnerController is null, Print Log
	{
		UE_LOG(LogTemp, Warning, TEXT("[UPRInteractComponent::BeginPlay] : OwnerController is null"));
	}

	// Set OwnerCam
	OwnerCam = OwnerController->PlayerCameraManager;
}



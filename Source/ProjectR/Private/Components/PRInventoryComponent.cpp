// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PRInventoryComponent.h"

#include "Camera/CameraComponent.h"
#include "Camera/CameraModifier_CameraShake.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UPRInventoryComponent::UPRInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPRInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UPRInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (OwnerCam)
	{	
		FVector CamLoc;
		FRotator CamRot;
		FHitResult HitResult;

		OwnerCam->GetCameraViewPoint(CamLoc, CamRot);

		GetWorld()->LineTraceSingleByChannel(HitResult, CamLoc, CamLoc + 1000.0f * CamRot.Vector(), ECollisionChannel::ECC_Visibility);
	}
}

void UPRInventoryComponent::InitComp()
{
	const ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());

	if (!OwnerChar) // If OwnerCharacter is null, Print Log
	{
		UE_LOG(LogTemp, Warning, TEXT("[UPRInventoryComponent::BeginPlay] : OwnerChar is null"));
	}

	OwnerController = Cast<APlayerController>(OwnerChar->GetController());

	if (!OwnerController) // If OwnerController is null, Print Log
	{
		UE_LOG(LogTemp, Warning, TEXT("[UPRInventoryComponent::BeginPlay] : OwnerController is null"));
	}

	// Set OwnerCam
	OwnerCam = OwnerController->PlayerCameraManager;
}



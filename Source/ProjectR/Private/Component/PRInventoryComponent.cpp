// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PRInventoryComponent.h"

#include "Library/RyanLibrary.h"

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

	URyanLibrary::SetupInputs(this, Cast<APlayerController>(GetOwner()), InventoryInputMappingContext);
}


// Called every frame
void UPRInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


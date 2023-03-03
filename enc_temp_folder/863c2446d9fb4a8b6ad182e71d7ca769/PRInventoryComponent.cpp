// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PRInventoryComponent.h"

#include "Component/PRInteractComponent.h"
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


void UPRInventoryComponent::IA_OpenInventory(const FInputActionValue& Value)
{
	if (UPRInteractComponent* InteractComp = Cast<UPRInteractComponent>(GetOwner()->GetComponentByClass(UPRInteractComponent::StaticClass())))
	{
		if(Value.Get<bool>())
		{
			//IPRWidgetInterface::Execute_UpdateWidgetMode(InteractComp->HUD, EInputMode::Inventory);
			UE_LOG(LogTemp, Warning, TEXT("Open Inventory"));
		}
		else
		{
			//IPRWidgetInterface::Execute_UpdateWidgetMode(InteractComp->HUD, EWidgetMode::Normal);
			UE_LOG(LogTemp, Warning, TEXT("Close Inventory"));
		}
	
	}
}
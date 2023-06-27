// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PRBaseComponent.h"

#include "Character/PRPlayerController.h"
#include "Interface/PRWidgetInterface.h"

// Sets default values for this component's properties
UPRBaseComponent::UPRBaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UPRBaseComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UPRBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPRBaseComponent::OnControllerInitialized(APlayerController* PlayerController)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s OnControllerInitialized"), *this->GetName());
}



// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PRInventoryComponent.generated.h"


UCLASS(Blueprintable, BlueprintType)
class PROJECTR_API UPRInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPRInventoryComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// Initialize Component
	void InitComp();

	// Character Camera Refs
	TObjectPtr<APlayerCameraManager> OwnerCam = nullptr;

	// Character Controller Refs
	TObjectPtr<APlayerController> OwnerController = nullptr;
		
};

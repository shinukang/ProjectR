// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Widget/PRWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "InputMappingContext.h"
#include "PRInteractComponent.generated.h"


UCLASS(Blueprintable, BlueprintType)
class PROJECTR_API UPRInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPRInteractComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void OnPlayerControllerInitialized(APlayerController* PlayerController);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Currently detected actor reference
	TObjectPtr<AActor> DetectedActor = nullptr;

private:
	// Check the interactive actor
	AActor* CheckInteractiveActor();

	// Character camera reference
	TObjectPtr<APlayerCameraManager> OwnerCam = nullptr;

	// HUD reference
	TObjectPtr<UPRWidgetBase> HUD = nullptr;

};

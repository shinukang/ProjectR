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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetupWidget();

private:
	// On Interact Action 
	UFUNCTION()
	void IA_Interact(const FInputActionValue& Value);

public:
	// HUD Ref
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	TObjectPtr<UPRWidgetBase> HUD = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> InteractInputMappingContext = nullptr;

private:
	// Check the interactive actor
	AActor* CheckInteractiveActor();

	// Character Camera Refs
	TObjectPtr<APlayerCameraManager> OwnerCam = nullptr;

	// Detected Actor Refs
	TObjectPtr<AActor> DetectedActor = nullptr;
};

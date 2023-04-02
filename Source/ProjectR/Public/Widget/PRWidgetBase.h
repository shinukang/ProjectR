// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/PRWidgetInterface.h"
#include "Library/PRItemStructLibrary.h"
#include "PRWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UPRWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateInteractInfo(FPRItemData ObjectData, int32 ObjectAmount);

	UFUNCTION(BlueprintImplementableEvent)
	void SetInventoryVisibility(bool bShow);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSelectedSlot(float Angle);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSlotData(const TArray<FPRInventorySlotData>& Inventory, int32 InventoryMaxSize);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHealthPointBar(float HealthPoint);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateStaminaBar(float Stamina);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/PRWidgetInterface.h"
#include "PRWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UPRWidgetBase : public UUserWidget, public IPRWidgetInterface
{
	GENERATED_BODY()

public:
	void UpdateInteractInfo_Implementation(const FName& ObjectID) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetInventoryVisibility(bool bShow);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSelectedSlot(float Angle);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSlotData(const TArray<FPRInventorySlotData>& Inventory, int32 InventoryMaxSize);
};

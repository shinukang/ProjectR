// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "System/PRItemObject.h"
#include "Interface/PRWidgetInterface.h"
#include "Library/PRItemStructLibrary.h"
#include "Library/PRItemEnumLibrary.h"
#include "Item/PRItem.h"
#include "Item/PRFirearm.h"
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
	void UpdateInteractInfo(const FText& DisplayName);

	UFUNCTION(BlueprintImplementableEvent)
	void SetInventoryVisibility(bool bShow);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateGroundItems(const TArray<APRItem*>& GroundItems);

	UFUNCTION(Client, Reliable)
	void Client_UpdateInventoryItems(const TArray<FPRItemData>& InventoryItems);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateInventoryItems(const TArray<FPRItemData>& InventoryItems);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateFirearm(int32 Index, FPRItemData ItemData);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateEquipment(FPRItemData ItemData);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHealthPointBar(float HealthPoint);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateStaminaBar(float Stamina);
};

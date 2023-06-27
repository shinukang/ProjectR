// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/PRWidgetInterface.h"
#include "Library/PRItemLibrary.h"
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

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateInventoryItems(const TArray<FPRItemData>& InventoryItems);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateFirearms(const TArray<APRFirearm*>& Firearms);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateEquipment(EPRCategory SubCategory, FName ID);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHealthPointBar(float HealthPoint);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateStaminaBar(float Stamina);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateShowingBullet(FName ID);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAttachment(int32 Index, EPRCategory SubCategory, FName ID);

	UFUNCTION(BlueprintImplementableEvent)
	void SetLiveCharacterMaterial(UMaterialInterface* NewLiveCharacterMaterial);

	UFUNCTION(BlueprintImplementableEvent)
	UMaterialInterface* GetLiveCharacterMaterial();
};

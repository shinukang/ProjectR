// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PRWidgetBase.h"

void UPRWidgetBase::Client_UpdateInventoryItems_Implementation(const TArray<FPRItemData>& InventoryItems)
{
	UE_LOG(LogTemp, Warning, TEXT("Client_UpdateInventoryItems"));
	UpdateInventoryItems(InventoryItems);
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Item/PRItemDataObject.h"
#include "PRWidgetInterface.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnResetDroppableSlot)

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPRWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTR_API IPRWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact")
	void UpdateInteractInfo(const FName& ObjectID);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Inventory")
	UPRWidgetBase* GetHUD();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	void UpdateDroppableSlot(UPRItemDataObject* ItemDataObject);

	FOnResetDroppableSlot OnResetDroppable;
};

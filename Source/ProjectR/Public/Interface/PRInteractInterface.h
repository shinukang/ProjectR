// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PRInteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPRInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTR_API IPRInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	bool CanInteract();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	FName GetItemID();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	int32 GetItemAmount();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	void OnInteract(APlayerController* PlayerController);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Library/PRItemLibrary.h"
#include "PRItemDataObject.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnUpdateAmount);

UCLASS(Blueprintable, BlueprintType)
class PROJECTR_API UPRItemDataObject : public UObject
{
	GENERATED_BODY()

public:
	UPRItemDataObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	FPRItemData ItemData;

	UFUNCTION(BlueprintCallable)
	void AddItemToInventory(APlayerController* PlayerController, bool bNeedToDestroy);

	UFUNCTION(BlueprintCallable)
	void AddAttachmentToFirearm(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	void AddEquipmentToCharacter(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	void RemoveAttachmentFromFirearm(APlayerController* PlayerController, bool bNeedToSpawn);

	UFUNCTION(BlueprintCallable)
	void RemoveItemFromInventory(APlayerController* PlayerController, bool bNeedToSpawn);

	UFUNCTION(BlueprintCallable)
	void RemoveEquipmentFromCharacter(APlayerController* PlayerController, bool bNeedToSpawn);

	UFUNCTION(BlueprintCallable)
	void UseItem(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	void SwapFirearm(APlayerController* PlayerController, int32 NewIndex);

};

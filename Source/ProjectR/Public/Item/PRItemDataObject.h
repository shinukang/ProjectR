// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Library/PRItemStructLibrary.h"
#include "PRItemDataObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTR_API UPRItemDataObject : public UObject
{
	GENERATED_BODY()

public:
	UPRItemDataObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	FPRItemData ItemData;

	UFUNCTION(BlueprintCallable)
	FPRBaseData GetBaseData() { return ItemData.GetBaseData(); }
};

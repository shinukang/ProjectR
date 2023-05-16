// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Library/PRCharacterStructLibrary.h"
#include "UObject/NoExportTypes.h"
#include "PRCostumeObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTR_API UPRCostumeObject : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void OnSelect();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta= (ExposeOnSpawn = "true"))
	FPRCostume CostumeData;
};

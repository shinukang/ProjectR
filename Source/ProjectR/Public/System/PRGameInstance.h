// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Library/PRCharacterStructLibrary.h"
#include "PRGameInstance.generated.h"

/**
 * 
 */

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnCostumeChanged, FPRCostume);

UCLASS()
class PROJECTR_API UPRGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetCharacterCostume(FPRCostume CostumeData);

	UFUNCTION(BlueprintCallable)
	TArray<FPRCostume> GetCharacterCostume() { return CharacterCostumeSetting; }

private:
	TArray<FPRCostume> CharacterCostumeSetting;

};

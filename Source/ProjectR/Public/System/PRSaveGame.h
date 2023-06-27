// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Library/PRCharacterStructLibrary.h"
#include "PRSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UPRSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FPRCostume> Costumes;
};

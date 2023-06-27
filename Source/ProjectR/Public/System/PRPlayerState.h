// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PRPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API APRPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	APRPlayerState();

	UFUNCTION(BlueprintCallable)
	virtual void SetPlayerName(const FString& NewPlayerName) override;

	UFUNCTION(BlueprintCallable)
	void SaveCostume();

	void LoadCostume();

	void UpdateCostume(FPRCostume Costume);

	void ApplyCostume();

	UPROPERTY()
	TArray<FPRCostume> Costumes;
};
	



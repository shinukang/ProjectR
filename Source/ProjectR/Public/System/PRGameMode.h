// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PRGameMode.generated.h"

/**
 * 
 */

UCLASS()
class PROJECTR_API APRGameMode : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	virtual void OnPostLogin(AController* NewPlayer) override;

	UFUNCTION(Server, Reliable)
	void Server_SpawnItems();


};

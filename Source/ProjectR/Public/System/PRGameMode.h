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

public:
	virtual void BeginPlay() override;

private:

	UFUNCTION(Server, Reliable)
	void Server_InitItems();
};

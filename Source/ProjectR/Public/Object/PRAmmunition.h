// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/PRConsumable.h"
#include "PRAmmunition.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API APRAmmunition : public APRConsumable
{
	GENERATED_BODY()

public:
	APRAmmunition();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual void Init() override;

	virtual void OnRep_ID() override;
};

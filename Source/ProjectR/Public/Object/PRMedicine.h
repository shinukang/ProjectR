// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/PRConsumable.h"
#include "PRMedicine.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API APRMedicine : public APRConsumable
{
	GENERATED_BODY()

	APRMedicine();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual void Init() override;

	virtual void OnRep_ID() override;
};

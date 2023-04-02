// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/PRItem.h"
#include "PRConsumable.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API APRConsumable : public APRItem
{
	GENERATED_BODY()

public:
	APRConsumable();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual void Init() override;

	virtual void OnRep_ID() override;	
};

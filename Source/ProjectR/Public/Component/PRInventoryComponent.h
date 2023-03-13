// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "PRBaseComponent.h"
#include "Components/ActorComponent.h"
#include "Library/PRObjectStructLibrary.h"
#include "Widget/PRWidgetBase.h"
#include "PRInventoryComponent.generated.h"

UCLASS(Blueprintable, BlueprintType)
class PROJECTR_API UPRInventoryComponent : public UPRBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPRInventoryComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool TryAddToInventory(FName ObjectID, int32 ObjectAmount);

	void UpdateInventorySlots();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UDataTable> ObjectDataTable = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FPRInventorySlotData> Inventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxSize = 0;
};

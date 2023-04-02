// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "PRBaseComponent.h"
#include "Components/ActorComponent.h"

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

	bool TryAddToInventory(FName ID, int32 Amount);

	void UpdateInventorySlots();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FPRInventorySlotData> Weapons;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FPRInventorySlotData> Consumables;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxCountOfWeapon = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxCountOfConsumable = 8;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxCapacity = 40.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CurrentCapacity = 0.0f;

private:
	bool bIsInventoryOpen = false;
};

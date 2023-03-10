// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "Components/ActorComponent.h"
#include "Library/PRObjectStructLibrary.h"
#include "Widget/PRWidgetBase.h"
#include "PRInventoryComponent.generated.h"

UCLASS(Blueprintable, BlueprintType)
class PROJECTR_API UPRInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPRInventoryComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//
	bool TryAddToInventory(FName ObjectID, int32 ObjectAmount);

	void UpdateInventorySlots();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnPlayerControllerInitialized(APlayerController* PlayerController);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UDataTable> ObjectDataTable = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FPRInventorySlotData> Inventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	uint8 MaxSize = 0;

private:
	// HUD reference
	TObjectPtr<UPRWidgetBase> HUD = nullptr;	
};

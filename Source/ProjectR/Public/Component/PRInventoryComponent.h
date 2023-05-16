// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "PRBaseComponent.h"
#include "Components/ActorComponent.h"
#include "System/PRItemObject.h"
#include "Item/PRItem.h"
#include "Item/PRFirearm.h"
#include "Widget/PRWidgetBase.h"
#include "PRInventoryComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnUpdateInteractInfo, const FText&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateEquipment, FPRItemData);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateGroundItems, const TArray<APRItem*>&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateInventoryItems, const TArray<FPRItemData>&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdateFirearm, int32, FPRItemData);

UCLASS(Blueprintable, BlueprintType)
class PROJECTR_API UPRInventoryComponent : public UPRBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPRInventoryComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnControllerInitialized(APlayerController* PlayerController) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool AddItemToInventory(APRItem* Item, int32 Index = INDEX_NONE);

	bool RemoveFromInventory(FName ID, int32 Amount);

	UFUNCTION(Client, Reliable)
	void Client_UpdateInventoryItems(const TArray<FPRItemData>& InInventoryItems);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateEquipment(FPRItemData ItemData);

	UFUNCTION(Client, Reliable)
	void Client_UpdateFirearm(int32 Index, FPRItemData ItemData);

	UFUNCTION(Server, Reliable)
	void OnInteractAction();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnRep_DetectedItem();

public:
	// Currently detected actor reference
	UPROPERTY(ReplicatedUsing=OnRep_DetectedItem)
	TObjectPtr<APRItem> DetectedItem = nullptr;

	//
	

	bool bIsInventoryOpen = false;

	FOnUpdateEquipment OnUpdateEquipment;

	FOnUpdateInventoryItems OnUpdateInventoryItems;

	FOnUpdateGroundItems OnUpdateGroundItems;

	FOnUpdateInteractInfo OnUpdateInteractInfo;

	FOnUpdateFirearm OnUpdateFirearm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	TArray<FPRItemData> Firearms;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	TArray<TObjectPtr<APRItem>> GroundedItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	TArray<FPRItemData> InventoryItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<EPRSubCategory, FPRItemData> Equipments;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxCountOfWeapon = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxCountOfConsumable = 8;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxCapacity = 40.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CurrentCapacity = 0.0f;

private:
	// Check the interactive actor
	APRItem* FindAimedItem();

	// Find Grounded Items
	void FindGroundItems();
	
	int32 GetValidFirearmSlotIndex(EPRSubCategory Category);

	// Character camera reference
	TObjectPtr<APlayerCameraManager> Camera = nullptr;

	int32 CurrentFirearmIndex = INDEX_NONE;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "PRBaseComponent.h"
#include "Components/ActorComponent.h"
#include "Item/PRItemDataObject.h"
#include "Item/PRItem.h"
#include "Item/PRFirearm.h"
#include "Widget/PRWidgetBase.h"
#include "PRInventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateInteractInfo, const FText&);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateGroundItems, const TArray<APRItem*>&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateInventoryItems, const TArray<FPRItemData>&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateFirearms, const TArray<APRFirearm*>&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdateEquipment, EPRCategory, FName);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateCapacity, float);

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

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	UFUNCTION(Server, Reliable)
	void Server_SwapFirearm(int32 PrevIndex, int32 NewIndex);

	UFUNCTION(Server, Reliable)
	void Server_AddItemToInventory(APRItem* Item, int32 Index = -1);

	UFUNCTION(Server, Reliable)
	void Server_AddItemToInventoryWithoutDestroy(FPRItemData ItemData);

	bool TryAddItemToInventory(FPRItemData ItemDataToAdd);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_RemoveFromInventory(FPRItemData ItemDataToRemove, bool bNeedToSpawn);

	bool TryRemoveFromInventory(FPRItemData ItemDataToRemove);

	UFUNCTION(Client, Reliable)
	void Client_UpdateEquipment(EPRCategory Category, UPRItemDataObject* ItemDataObject);

	void OnInteractAction();

	FPRItemData* GetInventoryItem(FPRItemData ItemDataToSearch);

	FPRItemData* GetInventoryItem(FName IDToSearch);

	FVector GetSpawnLocation();

	void SetMaxCapacity(float NewMaxCapacity);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Currently detected actor reference
	TObjectPtr<APRItem> DetectedItem = nullptr;

	bool bIsInventoryOpen = false;

	FOnUpdateEquipment OnUpdateEquipment;

	FOnUpdateInventoryItems OnUpdateInventoryItems;

	FOnUpdateGroundItems OnUpdateGroundItems;

	FOnUpdateInteractInfo OnUpdateInteractInfo;

	FOnUpdateFirearms OnUpdateFirearms;

	FOnUpdateCapacity OnUpdateCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_Firearms)
	TArray<TObjectPtr<APRFirearm>> Firearms;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<EPRCategory, FName> Equipments;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_GroundedItems)
	TArray<APRItem*> GroundedItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_InventoryItems)
	TArray<FPRItemData> InventoryItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxCountOfWeapon = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxCapacity = 40.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, ReplicatedUsing=OnRep_CurrentCapacity)
	float CurrentCapacity = 0.0f;

private:

	// Check the interactive actor
	UFUNCTION(Client, Reliable)
	void Client_FindAimedItem();

	APRItem* FindAimedItem();

	void FindGroundItems();

	// Find Grounded Items
	UFUNCTION(Server, Reliable)
	void Server_FindGroundItems();
	
	int32 GetValidFirearmSlotIndex(EPRCategory Category);

	UFUNCTION()
	void OnRep_Firearms();

	UFUNCTION()
	void OnRep_InventoryItems();

	UFUNCTION()
	void OnRep_GroundedItems();

	UFUNCTION()
	void OnRep_CurrentCapacity();

	// Character camera reference
	TObjectPtr<APlayerCameraManager> Camera = nullptr;

	int32 CurrentFirearmIndex = INDEX_NONE;
};

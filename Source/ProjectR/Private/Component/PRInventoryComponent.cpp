// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PRInventoryComponent.h"
#include "Library/PRItemStructLibrary.h"
#include "Component/PRInteractComponent.h"
#include "Library/RyanLibrary.h"

// Sets default values for this component's properties
UPRInventoryComponent::UPRInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UPRInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	//Inventory.Init(FPRInventorySlotData(), MaxSize);
	UpdateInventorySlots();
}

// Called every frame
void UPRInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
bool UPRInventoryComponent::TryAddToInventory(FName ID, int32 Amount)
{
	if (UDataTable* ItemDataTable = URyanLibrary::GetDataTable())
	{
		if (FPRItemData* ItemData = ItemDataTable->FindRow<FPRItemData>(ID, TEXT("")))
		{
			if(CurrentCapacity + ItemData->WeightPerPiece * Amount > MaxCapacity) return false;
			
			switch (ItemData->Type)
			{
			case EPRItemType::Default:
				return false;

			case EPRItemType::Firearm:
				if (Weapons.Num() < MaxCountOfWeapon)
				{
					Weapons.Add(FPRInventorySlotData(ID, 1));
					return true;
				}
				return false;
			case EPRItemType::Ammunition:
			case EPRItemType::Medicine:
				const FPRInventorySlotData ConsumableSlotData = FPRInventorySlotData(ID, Amount);
				if(const int32 Index = Consumables.Find(ConsumableSlotData) != INDEX_NONE)
				{
					Consumables[Index-1].Amount += Amount;
					return true;
				}
				if(Consumables.Num() < MaxCountOfConsumable)
				{
					Consumables.Add(ConsumableSlotData);
					return true;
				}
			}
		}
	}
	return false;
}

void UPRInventoryComponent::UpdateInventorySlots()
{
	//HUD->UpdateSlotData(Inventory, MaxSize);
}
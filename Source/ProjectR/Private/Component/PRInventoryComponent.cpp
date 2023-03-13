// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PRInventoryComponent.h"
#include "Library/PRObjectStructLibrary.h"
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
	Inventory.Init(FPRInventorySlotData(), MaxSize);
	UpdateInventorySlots();
}

// Called every frame
void UPRInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UPRInventoryComponent::TryAddToInventory(FName ObjectID, int32 ObjectAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("TryAddToInventory"));

	// ������ ���̺� �����ϴ� ID���� Ȯ��
	if (const FPRObject* ObjectData = ObjectDataTable->FindRow<FPRObject>(ObjectID, FString()))
	{
		const FPRInventorySlotData SlotData = FPRInventorySlotData(ObjectID, ObjectAmount);

		// �κ��丮 ���� ���� ID�� ���� ������Ʈ�� �����ϸ�
		int32 Index = Inventory.Find(SlotData);

		if(Index != INDEX_NONE)
		{
			if (ObjectData->bIsStackable)
			{
				Inventory[Index].Amount += ObjectAmount;
				UpdateInventorySlots();
				return true;
			}
			return false;
		}
		// �������� ������, �� ���� �� ���� ���� �ε����� ã�´�.
		Index = Inventory.Find(FPRInventorySlotData());
		if (Index != INDEX_NONE)
		{
			Inventory[Index] = SlotData;// ������Ʈ �߰�
			UpdateInventorySlots();
			return true;
		}
	}
	return false;
}

void UPRInventoryComponent::UpdateInventorySlots()
{
	HUD->UpdateSlotData(Inventory, MaxSize);
}
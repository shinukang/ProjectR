// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PRItemDataObject.h"

#include "Character/PRCharacter.h"
#include "Component/PRInventoryComponent.h"
#include "Net/UnrealNetwork.h"

UPRItemDataObject::UPRItemDataObject()
{
	
}

void UPRItemDataObject::AddItemToInventory(APlayerController* PlayerController, bool bNeedToDestroy)
{
	if(PlayerController)
	{
		if(UPRInventoryComponent* InventoryComponent = Cast<UPRInventoryComponent>(PlayerController->GetPawn()->GetComponentByClass(UPRInventoryComponent::StaticClass())))
		{
			if(bNeedToDestroy)
			{
				if (APRItem* Item = Cast<APRItem>(GetOuter()))
				{
					InventoryComponent->Server_AddItemToInventory(Item, ItemData.Index);
				}
			}
			else
			{
				InventoryComponent->Server_AddItemToInventoryWithoutDestroy(ItemData);
			}
		}
	}
}
void UPRItemDataObject::AddAttachmentToFirearm(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		if (UPRInventoryComponent* InventoryComponent = Cast<UPRInventoryComponent>(PlayerController->GetPawn()->GetComponentByClass(UPRInventoryComponent::StaticClass())))
		{
			if (APRFirearm* FirearmToAttachItem = InventoryComponent->Firearms[ItemData.Index])
			{
				FirearmToAttachItem->Server_AddAttachment(ItemData.Category, ItemData.ID, Cast<APRItem>(GetOuter()));
				RemoveItemFromInventory(PlayerController, false);
			}
		}
	}
}
void UPRItemDataObject::AddEquipmentToCharacter(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		if (APRCharacter* Character = Cast<APRCharacter>(PlayerController->GetPawn()))
		{
			Character->Server_UpdateEquipment(ItemData.Category, ItemData.ID, Cast<APRItem>(GetOuter()));
		}
	}
}
void UPRItemDataObject::RemoveItemFromInventory(APlayerController* PlayerController, bool bNeedToSpawn)
{
	if (PlayerController)
	{
		if (APRCharacter* Character = Cast<APRCharacter>(PlayerController->GetPawn()))
		{
			Character->PRInventoryComponent->Server_RemoveFromInventory(ItemData, bNeedToSpawn);
		}
	}
}
void UPRItemDataObject::RemoveAttachmentFromFirearm(APlayerController* PlayerController, bool bNeedToSpawn)
{
	if (PlayerController)
	{
		if (UPRInventoryComponent* InventoryComponent = Cast<UPRInventoryComponent>(PlayerController->GetPawn()->GetComponentByClass(UPRInventoryComponent::StaticClass())))
		{
			if(ItemData.Index == INDEX_NONE)
			{
				return;	
			}

			if (APRFirearm* FirearmToAttachItem = InventoryComponent->Firearms[ItemData.Index])
			{
				FirearmToAttachItem->Server_RemoveAttachment(ItemData.Category, bNeedToSpawn);
			}
		}
	}
}
void UPRItemDataObject::RemoveEquipmentFromCharacter(APlayerController* PlayerController, bool bNeedToSpawn)
{
	if (PlayerController)
	{
		if (APRCharacter* Character = Cast<APRCharacter>(PlayerController->GetPawn()))
		{
			Character->Server_UpdateEquipment(ItemData.Category, NAME_None);
		}
	}
}
void UPRItemDataObject::UseItem(APlayerController* PlayerController)
{
	if(ItemData.Category == EPRCategory::Medicine_Stamina || ItemData.Category == EPRCategory::Medicine_HealthPoint)
	{
		if (PlayerController)
		{
			if (APRCharacter* Character = Cast<APRCharacter>(PlayerController->GetPawn()))
			{
				Character->PRInventoryComponent->Server_RemoveFromInventory(ItemData, false);
				Character->PRStatusComponent->Server_UseMedicine(ItemData);
			}
		}
	}
}

void UPRItemDataObject::SwapFirearm(APlayerController* PlayerController, int32 NewIndex)
{
	if (PlayerController)
	{
		if (APRCharacter* Character = Cast<APRCharacter>(PlayerController->GetPawn()))
		{
			Character->PRInventoryComponent->Server_SwapFirearm(ItemData.Index, NewIndex);
		}
	}
}




// Fill out your copyright notice in the Description page of Project Settings.

#include "System/PRItemObject.h"

#include "Character/PRPlayerController.h"
#include "Item/PRItem.h"
#include "Library/RyanLibrary.h"


void UPRItemObject::Init()
{
	if(const APRItem* Item = Cast<APRItem>(GetOuter()))
	{
		ID = Item->GetID();

		if (!URyanLibrary::IsValidItemID(ID)) return;

		BaseData = FPRBaseData(ID);

		if (const UDataTable* DataTable = URyanLibrary::GetItemDataTable())
		{
			Amount = 1;

			SubCategory = BaseData.Category;

			switch(SubCategory)
			{
			case EPRSubCategory::Attachment_Barrel:
			case EPRSubCategory::Attachment_Grip:
			case EPRSubCategory::Attachment_Scope:
				MainCategory = EPRMainCategory::Attachment;
				break;

			case EPRSubCategory::Equipment_Backpack:
			case EPRSubCategory::Equipment_HeadGear:
			case EPRSubCategory::Equipment_Vest:
				MainCategory = EPRMainCategory::Equipment;
				break;

			case EPRSubCategory::Firearm_Primary:
			case EPRSubCategory::Firearm_Secondary:
				MainCategory = EPRMainCategory::Firearm;
				break;
				 
			case EPRSubCategory::Ammunition_Default:
				MainCategory = EPRMainCategory::Ammunition;
				break;

			case EPRSubCategory::Medicine_Default:
				MainCategory = EPRMainCategory::Medicine;
				break;

			case EPRSubCategory::Default:
				MainCategory = EPRMainCategory::Default;
				break;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Outer is not a APItem"));
	}
}

void UPRItemObject::Init(int32 InAmount)
{
	Init();

	if (InAmount >= 1) Amount = InAmount;
}

void UPRItemObject::AddItemToInventory(APlayerController* PlayerController, int32 Index)
{
	if(UPRInventoryComponent* InventoryComponent = Cast<UPRInventoryComponent>(PlayerController->GetComponentByClass(UPRInventoryComponent::StaticClass())))
	{
		/*
		if(InventoryComponent->AddItemToInventory(this, Index))
		{
			if (APRItem* Item = Cast<APRItem>(this->GetOuter()))
			{
				if(Item->HasAuthority())
				{
					Item->Server_DestroyItem(InventoryComponent);
				}
			}
		}
		*/
	}
}




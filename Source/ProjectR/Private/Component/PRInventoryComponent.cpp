// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PRInventoryComponent.h"

#include "Character/PRCharacter.h"
#include "Engine/ActorChannel.h"
#include "Character/PRPlayerController.h"
#include "Library/PRItemLibrary.h"
#include "Library/RyanLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPRInventoryComponent::UPRInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	Firearms.Init(nullptr, MaxCountOfWeapon);
}

void UPRInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UPRInventoryComponent, InventoryItems, COND_None);
	DOREPLIFETIME(UPRInventoryComponent, Firearms);
	DOREPLIFETIME(UPRInventoryComponent, CurrentCapacity);
}

// Called when the game starts
void UPRInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
}

void UPRInventoryComponent::OnControllerInitialized(APlayerController* PlayerController)
{
	Super::OnControllerInitialized(PlayerController);
	Camera = PlayerController->PlayerCameraManager;
	PrimaryComponentTick.bCanEverTick = true;
}

// Called every frame
void UPRInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateAimedItemInfo();
}

void UPRInventoryComponent::Server_AddItemToInventory_Implementation(APRItem* Item, int32 Index)
{
	if(Item)
	{
		FPRItemData ItemData = Item->ItemData;
		ItemData.Index = Index;

		if (TryAddItemToInventory(ItemData))
		{
			Item->Destroy();
		}
	}
}

void UPRInventoryComponent::Server_AddItemToInventoryWithoutDestroy_Implementation(FPRItemData ItemData)
{
	TryAddItemToInventory(ItemData);
}

bool UPRInventoryComponent::TryAddItemToInventory(FPRItemData ItemDataToAdd)
{
	const float NewItemWeight = ItemDataToAdd.Amount * UPRItemLibrary::GetBaseData(ItemDataToAdd.ID).WeightPerPiece;

	if(CurrentCapacity + NewItemWeight > MaxCapacity)
	{
		return false;
	}

	int32 SelectedIndex = ItemDataToAdd.Index;

	switch(ItemDataToAdd.Category)
	{
	case EPRCategory::Default:
		return false;
		
	case EPRCategory::Firearm_AssaultRifle:
	case EPRCategory::Firearm_Shotgun:
	case EPRCategory::Firearm_SubMachineGun:
	case EPRCategory::Firearm_SniperRifle:
	case EPRCategory::Firearm_Pistol:

		if (SelectedIndex == INDEX_NONE)
		{
			SelectedIndex = GetValidFirearmSlotIndex(ItemDataToAdd.Category);

			if (SelectedIndex == INDEX_NONE)
			{
				return false;
			}
		}

		if(APRCharacter* Character = Cast<APRCharacter>(GetOwner()))
		{
			if(APRFirearm* Firearm = Firearms[SelectedIndex])
			{
				Server_RemoveFromInventory(Firearm->ItemData, true);
			}
			Character->Server_UpdateFirearm(SelectedIndex, ItemDataToAdd);
			return true;
		}
		return false;

	case EPRCategory::Equipment_HeadGear:
	case EPRCategory::Equipment_Vest:
	case EPRCategory::Equipment_Backpack:

		if (APRCharacter* Character = Cast<APRCharacter>(GetOwner()))
		{
			Character->Server_UpdateEquipment(ItemDataToAdd.Category, ItemDataToAdd.ID);
			return true;
		}
		return false;

	case EPRCategory::Ammunition:
	case EPRCategory::Medicine_HealthPoint:
	case EPRCategory::Medicine_Stamina:
		for (int32 NewIndex = 0; NewIndex < InventoryItems.Num(); NewIndex++)
		{
			if (InventoryItems[NewIndex].ID == ItemDataToAdd.ID)
			{
				InventoryItems[NewIndex].Amount += ItemDataToAdd.Amount;
				return true;
			}
		}
	case EPRCategory::Attachment_Barrel:
	case EPRCategory::Attachment_Grip:
	case EPRCategory::Attachment_Scope:
		InventoryItems.Add(ItemDataToAdd);
		return true;
	}
	return false;
}

void UPRInventoryComponent::Server_RemoveFromInventory_Implementation(FPRItemData ItemDataToRemove, bool bNeedToSpawn)
{
	if (TryRemoveFromInventory(ItemDataToRemove))
	{
		CurrentCapacity -= ItemDataToRemove.Amount * UPRItemLibrary::GetBaseData(ItemDataToRemove.ID).WeightPerPiece;

		if (bNeedToSpawn)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			FVector SpawnLocation = GetSpawnLocation();
			FRotator SpawnRotation = FRotator::ZeroRotator;

			if (APRItem* Item = GetWorld()->SpawnActor<APRItem>(SpawnLocation, SpawnRotation, SpawnParameters))
			{
				Item->Init(ItemDataToRemove);
			}
		}
	}
}

bool UPRInventoryComponent::TryRemoveFromInventory(FPRItemData ItemDataToRemove)
{
	switch (ItemDataToRemove.Category)
	{
	case EPRCategory::Default:
		return false;

	case EPRCategory::Firearm_AssaultRifle:
	case EPRCategory::Firearm_Shotgun:
	case EPRCategory::Firearm_SubMachineGun:
	case EPRCategory::Firearm_SniperRifle:
	case EPRCategory::Firearm_Pistol:
		if(APRFirearm* FirearmToRemove = Firearms[ItemDataToRemove.Index])
		{
			TArray<EPRCategory> Attachments;

			FirearmToRemove->Attachments.GetKeys(Attachments);

			for (EPRCategory Category : Attachments)
			{
				FirearmToRemove->Server_RemoveAttachment(Category, false);
			}

			if (FirearmToRemove->LoadedBullets > 0)
			{
				Server_AddItemToInventoryWithoutDestroy(FPRItemData(FirearmToRemove->FirearmData.AmmunitionID, FirearmToRemove->LoadedBullets));
			}

			Firearms[ItemDataToRemove.Index] = nullptr;
			FirearmToRemove->Destroy();

			if(APRCharacter* Character = Cast<APRCharacter>(GetOwner()))
			{
				if (FirearmToRemove == Character->GetCurrentHeldFirearm())
				{
					Character->SetCurrentHeldFirearm(nullptr);
				}
			}
			return true;
		}
		return false;
	case EPRCategory::Equipment_HeadGear:
	case EPRCategory::Equipment_Vest:
	case EPRCategory::Equipment_Backpack:
		if (UPRItemLibrary::IsValidItemID(ItemDataToRemove.ID))
		{
			if (APRItem* NewItem = GetWorld()->SpawnActor<APRItem>())
			{
				NewItem->Init(FPRItemData(ItemDataToRemove.ID));
				return true;
			}
			return false;
		}
		return false;
	case EPRCategory::Attachment_Barrel:
	case EPRCategory::Attachment_Scope:
	case EPRCategory::Attachment_Grip:
	case EPRCategory::Ammunition:
	case EPRCategory::Medicine_HealthPoint:
	case EPRCategory::Medicine_Stamina:
		const int32 Index = InventoryItems.Find(ItemDataToRemove);
		if (Index != INDEX_NONE)
		{
			InventoryItems[Index].Amount -= ItemDataToRemove.Amount;

			if (InventoryItems[Index].Amount <= 0)
			{
				InventoryItems.RemoveAt(Index);
			}
			OnRep_InventoryItems();
			OnUpdateInventoryItems.Broadcast(InventoryItems);
			return true;
		}
		return false;
	}
	return false;
}

void UPRInventoryComponent::OnInteractAction()
{
	Server_AddItemToInventory(DetectedItem);
}

void UPRInventoryComponent::UpdateAimedItemInfo()
{
	APRItem* NewDetectedItem = FindAimedItem();

	if (IsValid(NewDetectedItem) && Cast<APRCharacter>(GetOwner())->GetRotationMode() != EPRRotationMode::Aiming)
	{
		if (DetectedItem != NewDetectedItem)
		{
			// UI update
			DetectedItem = NewDetectedItem;
			OnUpdateInteractInfo.Broadcast(UPRItemLibrary::GetBaseData(DetectedItem->ItemData.ID).Name);
		}
	}
	else
	{
		if (DetectedItem != nullptr)
		{
			DetectedItem = nullptr;
			OnUpdateInteractInfo.Broadcast(FText());
		}
	}
}

APRItem* UPRInventoryComponent::FindAimedItem()
{
	if (Camera)
	{
		FVector CamLoc; // Player Camera Location
		FRotator CamRot; // Player Camera Rotation
		FHitResult HitResult;

		Camera->GetCameraViewPoint(CamLoc, CamRot);

		GetWorld()->LineTraceSingleByChannel(HitResult, CamLoc, CamLoc + 1000.0f * CamRot.Vector(), ECollisionChannel::ECC_Visibility);

		if (HitResult.bBlockingHit)
		{
			if (APRItem* AimedItem = Cast<APRItem>(HitResult.GetActor()))
			{
				return AimedItem;
			}
		}
	}
	return nullptr;
}

void UPRInventoryComponent::AddGroundItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(APRItem* Item = Cast<APRItem>(OtherActor))
	{
		GroundedItems.Add(Item);
	}
	OnUpdateGroundItems.Broadcast(GroundedItems);
}

void UPRInventoryComponent::RemoveGroundItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APRItem* Item = Cast<APRItem>(OtherActor))
	{
		GroundedItems.Remove(Item);
	}
	OnUpdateGroundItems.Broadcast(GroundedItems);
}

int32 UPRInventoryComponent::GetValidFirearmSlotIndex(EPRCategory Category)
{
	switch (Category)
	{
	case EPRCategory::Firearm_AssaultRifle:
	case EPRCategory::Firearm_Shotgun:
	case EPRCategory::Firearm_SubMachineGun:
	case EPRCategory::Firearm_SniperRifle:
		for (int32 Index = 0; Index <= 1; Index++)
		{
			// 빈 슬롯 중 인덱스가 작은 곳을 찾아서 반환
			if (!Firearms[Index]) return Index;
		}

		if(const APRFirearm* Firearm = Cast<APRCharacter>(GetOwner())->GetCurrentHeldFirearm())
		{
			return Firearm->ItemData.Index;
		}
		return INDEX_NONE;
	case EPRCategory::Firearm_Pistol:
		return 2;

	default:
		return INDEX_NONE;
	}
}

void UPRInventoryComponent::OnRep_Firearms()
{
	UE_LOG(LogClass, Warning, TEXT("OnRep_Firearms"));
	OnUpdateFirearms.Broadcast(Firearms);
}

void UPRInventoryComponent::OnRep_InventoryItems()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_InventoryItems"));
	OnUpdateInventoryItems.Broadcast(InventoryItems);
	Server_UpdateCapacity();
}

void UPRInventoryComponent::OnRep_CurrentCapacity()
{
	UE_LOG(LogTemp, Warning, TEXT("UPRInventoryComponent::OnRep_CurrentCapacity"));
	OnUpdateCapacity.Broadcast(CurrentCapacity/MaxCapacity);
}

void UPRInventoryComponent::Server_UpdateCapacity_Implementation()
{
	float NewCapacity = 0.0f;

	for (const FPRItemData ItemData : InventoryItems)
	{
		NewCapacity += ItemData.Amount * UPRItemLibrary::GetBaseData(ItemData.ID).WeightPerPiece;
	}

	CurrentCapacity = NewCapacity;
}

void UPRInventoryComponent::Client_UpdateEquipment_Implementation(EPRCategory SubCategory, UPRItemDataObject* ItemDataObject)
{
	/*
	UE_LOG(LogTemp, Warning, TEXT("Client_UpdateEquipment"));
	OnUpdateEquipment.Broadcast(SubCategory, ItemDataObject);
	*/
}

FPRItemData* UPRInventoryComponent::GetInventoryItem(FPRItemData ItemDataToSearch)
{
	for(int32 Index = 0; Index < InventoryItems.Num(); Index++)
	{
		if (InventoryItems[Index] == &ItemDataToSearch)
		{
			return &InventoryItems[Index];
		}
	}
	return nullptr;
}

FPRItemData* UPRInventoryComponent::GetInventoryItem(FName IDToSearch)
{
	for (int32 Index = 0; Index < InventoryItems.Num(); Index++)
	{
		if (InventoryItems[Index].ID == IDToSearch)
		{
			return &InventoryItems[Index];
		}
	}
	return nullptr;
}

FVector UPRInventoryComponent::GetSpawnLocation()
{
	FVector Start = GetOwner()->GetActorLocation() + GetOwner()->GetActorRotation().Vector() * 50.0f;
	FVector End = Start + FVector(0.0f, 0.0f, -200.0f);

	FHitResult HitResult;
	FVector SpawnLocation = End;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
	{
		SpawnLocation = HitResult.ImpactPoint;
	}
	return SpawnLocation;
}

void UPRInventoryComponent::SetMaxCapacity(float NewMaxCapacity)
{
	MaxCapacity = NewMaxCapacity;
	OnUpdateCapacity.Broadcast(CurrentCapacity/MaxCapacity);
}

void UPRInventoryComponent::Server_SwapFirearm_Implementation(int32 PrevIndex, int32 NewIndex)
{
	if(APRCharacter* Character = Cast<APRCharacter>(GetOwner()))
	{
		APRFirearm* TempFirearm = Firearms[NewIndex];

		Firearms[NewIndex] = Firearms[PrevIndex];
		Firearms[NewIndex]->ItemData.Index = NewIndex;
		Firearms[PrevIndex] = TempFirearm;

		Character->AttachToBack(Firearms[PrevIndex]);
		Character->AttachToBack(Firearms[NewIndex]);
	}
}

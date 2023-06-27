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
	DOREPLIFETIME(UPRInventoryComponent, GroundedItems);
	DOREPLIFETIME_CONDITION(UPRInventoryComponent, InventoryItems, COND_None);
	DOREPLIFETIME(UPRInventoryComponent, Firearms);
}

bool UPRInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	//bWroteSomething |= Channel->ReplicateSubobjectList(InventoryItems, *Bunch, *RepFlags);

	//bWroteSomething |= Channel->ReplicateSubobjectList(GroundedItems, *Bunch, *RepFlags);

	return bWroteSomething;
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
	Client_FindAimedItem();
	FindGroundItems();
}

void UPRInventoryComponent::Server_AddItemToInventory_Implementation(APRItem* Item, int32 Index)
{
	if(Item)
	{
		if (TryAddItemToInventory(Item->ItemData))
		{
			Item->Destroy();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UPRInventoryComponent::Server_AddItemToInventory : Failure to TryAddItemToInventory."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UPRInventoryComponent::Server_AddItemToInventory : Item or ItemDataObject is invalid."));
	}
}

/*
void UPRInventoryComponent::Server_AddItemToInventoryWithoutDestroy_Implementation(UPRItemDataObject* ItemDataObject)
{
	if (ItemDataObject)
	{
		TryAddItemToInventory(ItemDataObject);
	}
}
*/


bool UPRInventoryComponent::TryAddItemToInventory(FPRItemData ItemDataToAdd)
{
	int32 SelectedIndex = ItemDataToAdd.Index;

	switch(ItemDataToAdd.Category)
	{
	case EPRCategory::Default:
		return false;
		
	case EPRCategory::Firearm_Primary:
	case EPRCategory::Firearm_Secondary:
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
		if (bNeedToSpawn)
		{
			FVector SpawnLocation = GetSpawnLocation();
			FRotator SpawnRotation = FRotator::ZeroRotator;

			if (APRItem* Item = GetWorld()->SpawnActor<APRItem>(SpawnLocation, SpawnRotation))
			{
				Item->Init(ItemDataToRemove);
			}
		}
		else
		{

		}
	}
}

bool UPRInventoryComponent::TryRemoveFromInventory(FPRItemData ItemDataToRemove)
{
	switch (ItemDataToRemove.Category)
	{
	case EPRCategory::Default:
		return false;

	case EPRCategory::Firearm_Primary:
	case EPRCategory::Firearm_Secondary:
		//Firearms.
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
		int32 Index = InventoryItems.Find(ItemDataToRemove);

		if (Index != INDEX_NONE)
		{
			if (InventoryItems[Index].Amount -= ItemDataToRemove.Amount <= 0)
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

void UPRInventoryComponent::Client_FindAimedItem_Implementation()
{
	if (APRItem* NewDetectedItem = FindAimedItem())
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
void UPRInventoryComponent::FindGroundItems()
{
	if(bIsInventoryOpen)
	{
		Server_FindGroundItems();
	}
}

void UPRInventoryComponent::Server_FindGroundItems_Implementation()
{
	GroundedItems.Empty();

	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> HitResults;
	FVector SweepCenter = GetOwner()->GetActorLocation();

	GetWorld()->SweepMultiByChannel(HitResults, SweepCenter, SweepCenter, FQuat(0.0f), ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(500.0f));

	for (FHitResult HitResult : HitResults)
	{
		if (APRItem* DetectedGroundItem = Cast<APRItem>(HitResult.GetActor()))
		{
			GroundedItems.Add(DetectedGroundItem);
		}
	}
}

int32 UPRInventoryComponent::GetValidFirearmSlotIndex(EPRCategory Category)
{
	int32 StartIndex = 0;

	switch (Category)
	{
	case EPRCategory::Firearm_Primary:
	
		if (CurrentFirearmIndex != INDEX_NONE)
		{
			if (CurrentFirearmIndex <= 1)
			{
				return CurrentFirearmIndex;
			}
			return 2;
		}
		break;

	case EPRCategory::Firearm_Secondary:
		if (CurrentFirearmIndex != INDEX_NONE)
		{
			if (CurrentFirearmIndex > 1)
			{
				return CurrentFirearmIndex;
			}
			return 0;
		}
		StartIndex += 2;
		break;

	default:
		break;
	}

	for (int32 Index = StartIndex; Index < StartIndex + 2; Index++)
	{
		// 빈 슬롯 중 인덱스가 작은 곳을 찾아서 반환
		if (!Firearms[Index]) return Index;
	}
	// 모든 인덱스가 꽉 차있다면,
	return INDEX_NONE;
}

void UPRInventoryComponent::OnRep_InventoryItems()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_InventoryItems"));
	OnUpdateInventoryItems.Broadcast(InventoryItems);
}

void UPRInventoryComponent::OnRep_GroundedItems()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_GrounedItmes"));
	OnUpdateGroundItems.Broadcast(GroundedItems);
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PRInventoryComponent.h"

#include "Character/PRPlayerController.h"
#include "Library/PRItemStructLibrary.h"
#include "Library/RyanLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPRInventoryComponent::UPRInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	Firearms.Init(FPRItemData(), MaxCountOfWeapon);
}

void UPRInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPRInventoryComponent, DetectedItem);
	DOREPLIFETIME(UPRInventoryComponent, GroundedItems);
	DOREPLIFETIME(UPRInventoryComponent, InventoryItems);
	DOREPLIFETIME(UPRInventoryComponent, Firearms);
}


// Called when the game starts
void UPRInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
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

	int32 ItemAmount = -1;

	if (APRItem* NewDetectedItem = FindAimedItem())
	{
		if (DetectedItem != NewDetectedItem)
		{
			// UI update
			DetectedItem = NewDetectedItem;

			if (GetOwnerRole() != ROLE_Authority) // 클라이언트
			{
				OnUpdateInteractInfo.Execute(DetectedItem->GetItemData().GetBaseData()->Name);
			}
		}
	}
	else
	{
		// UI update (remove)
		if (DetectedItem != nullptr)
		{
			DetectedItem = nullptr;

			if (GetOwnerRole() != ROLE_Authority)
			{
				OnUpdateInteractInfo.Execute(FText());
			}
		}
	}
	FindGroundItems();
}


bool UPRInventoryComponent::AddItemToInventory(APRItem* Item, int32 Index)
{
	int32 SelectedIndex = Index;

	switch(Item->GetItemData().MainCategory)
	{
	case EPRMainCategory::Firearm:
		if (SelectedIndex == INDEX_NONE)
		{
			SelectedIndex = GetValidFirearmSlotIndex(Item->GetItemData().SubCategory);

			if (SelectedIndex == INDEX_NONE)
			{
				return false;
			}
		}
		Firearms[SelectedIndex] = Item->GetItemData();
		Client_UpdateFirearm(SelectedIndex, Item->GetItemData());
		return true;

	case EPRMainCategory::Equipment:
		Equipments.Add(Item->GetItemData().SubCategory, Item->GetItemData());
		Multicast_UpdateEquipment(Item->GetItemData());
		return true;

	case EPRMainCategory::Ammunition:
	case EPRMainCategory::Medicine:
		for (int32 NewIndex = 0; NewIndex < InventoryItems.Num(); NewIndex++)
		{
			if (InventoryItems[NewIndex] == Item->GetItemData())
			{
				InventoryItems[NewIndex].Amount += Item->GetItemData().Amount;
				Client_UpdateInventoryItems(InventoryItems);
				return true;
			}
		}
	case EPRMainCategory::Attachment:
		InventoryItems.Add(Item->GetItemData());
		Client_UpdateInventoryItems(InventoryItems);
		return true;

	default:
		break;
	}
	return false;
}

bool UPRInventoryComponent::RemoveFromInventory(FName ID, int32 Amount)
{
	return true;
}

void UPRInventoryComponent::OnInteractAction_Implementation()
{
	if(DetectedItem)
	{
		if (AddItemToInventory(DetectedItem))
		{
			DetectedItem->SetOwner(this->GetOwner());
			DetectedItem->DestroyItem();
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
		GroundedItems.Empty();
		
		TArray<AActor*> ActorsToIgnore;
		TArray<FHitResult> HitResults;
		FVector SweepCenter = GetOwner()->GetActorLocation();

		GetWorld()->SweepMultiByChannel(HitResults, SweepCenter, SweepCenter, FQuat(0.0f), ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(500.0f));

		for(FHitResult HitResult : HitResults)
		{
			if(APRItem* DetectedGroundItem = Cast<APRItem>(HitResult.GetActor()))
			{
				GroundedItems.Add(DetectedGroundItem);
			}
		}
		OnUpdateGroundItems.Broadcast(GroundedItems);
	}
}

int32 UPRInventoryComponent::GetValidFirearmSlotIndex(EPRSubCategory Category)
{
	int32 StartIndex = 0;

	switch (Category)
	{
	case EPRSubCategory::Firearm_Primary:
	
		if (CurrentFirearmIndex != INDEX_NONE)
		{
			if (CurrentFirearmIndex <= 1)
			{
				return CurrentFirearmIndex;
			}
			return 2;
		}
		break;

	case EPRSubCategory::Firearm_Secondary:
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
		if (!Firearms[Index].IsValid()) return Index;
	}
	// 모든 인덱스가 꽉 차있다면,
	return INDEX_NONE;
}


/*
void UPRInventoryComponent::Multicast_EquipFirearm_Implementation(APRFirearm* NewFirearm, int32 Index)
{
	if (!NewFirearm) return;

	if(Character)
	{
		if (APRFirearm* PreviousFirearm = Firearms[Index])
		{
			if(PreviousFirearm != NewFirearm)
			{
				PreviousFirearm->Destroy();
			}
		}
		const FString SocketString = FString::Printf(TEXT("Firearm_Socket_0%d"), Index + 1);
		const FName SocketName = FName(*SocketString);
		NewFirearm->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		UE_LOG(LogTemp, Warning, TEXT("EquipFirearm : %s"), *NewFirearm->GetName());
	}
}


void UPRInventoryComponent::HoldFirearm(int32 Index)
{
	if (Character)
	{
		for (int FirearmIndex = 0; FirearmIndex < Firearms.Num(); FirearmIndex++)
		{
			if (FirearmIndex == Index)
			{
				if (Firearms[FirearmIndex] != nullptr)
				{
					APRFirearm* SelectedFirearm = Firearms[Index];

					SelectedFirearm->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("VB LHS_ik_hand_gun")));

					const EPROverlayState OverlayState = SelectedFirearm->ItemData.GetAdvancedData<FPRFirearmData>()->OverlayState;

					Character->SetOverlayState(OverlayState);

					CurrentFirearmIndex = Index;
				}
				else
				{
					Character->SetOverlayState(EPROverlayState::Default);
				}
				
			}
			else
			{
				Multicast_EquipFirearm(Firearms[FirearmIndex], FirearmIndex);
			}
		}
	}
}
*/

void UPRInventoryComponent::OnRep_DetectedItem()
{
	if(GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnRep_DetectedItem : Owner has Authority"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnRep_DetectedItem : Owner doesn't have Authority"));
	}
}

void UPRInventoryComponent::Client_UpdateInventoryItems_Implementation(const TArray<FPRItemData>& InInventoryItems)
{
	//UE_LOG(LogTemp, Warning, TEXT("Client_UpdateInventoryItems"));
	InventoryItems = InInventoryItems;
	OnUpdateInventoryItems.Broadcast(InventoryItems);
}

void UPRInventoryComponent::Multicast_UpdateEquipment_Implementation(FPRItemData ItemData)
{
	//UE_LOG(LogTemp, Warning, TEXT("Client_UpdateEquipment"));
	Equipments.Add(ItemData.SubCategory, ItemData);
	OnUpdateEquipment.Broadcast(ItemData);
}

void UPRInventoryComponent::Client_UpdateFirearm_Implementation(int32 Index, FPRItemData ItemData)
{
	Firearms[Index] = ItemData;
	OnUpdateFirearm.Broadcast(Index, ItemData);
}

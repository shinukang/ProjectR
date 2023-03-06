// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PRInventoryComponent.h"

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
}

void UPRInventoryComponent::OnPlayerControllerInitialized(APlayerController* PlayerController)
{
	HUD = IPRWidgetInterface::Execute_GetHUD(PlayerController);
}

// Called every frame
void UPRInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPRInventoryComponent::TryAddToInventory(FName ObjectID, int32 ObjectAmount)
{
	// 데이터 테이블에 존재하는 ID인지 확인
	if (const FPRObject* ObjectData = ObjectDataTable->FindRow<FPRObject>(ObjectID, FString()))
	{
		if(Inventory.Contains(ObjectID))// 현재 인벤토리에 존재하면
		{
			if(ObjectData->bIsStackable) // 쌓을 수 있는 아이템인지 확인하고 맞다면
			{
				Inventory[ObjectID] += ObjectAmount; // 수량 추가
			}
		}
		// 존재하지 않으면
		Inventory.Add(ObjectID, ObjectAmount); // 오브젝트 추가
	}
}

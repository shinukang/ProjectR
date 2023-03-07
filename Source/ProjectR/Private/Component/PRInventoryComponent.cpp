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
	// ������ ���̺��� �����ϴ� ID���� Ȯ��
	if (const FPRObject* ObjectData = ObjectDataTable->FindRow<FPRObject>(ObjectID, FString()))
	{
		if(Inventory.Contains(ObjectID))// ���� �κ��丮�� �����ϸ�
		{
			if(ObjectData->bIsStackable) // ���� �� �ִ� ���������� Ȯ���ϰ� �´ٸ�
			{
				Inventory[ObjectID] += ObjectAmount; // ���� �߰�
			}
		}
		// �������� ������
		Inventory.Add(ObjectID, ObjectAmount); // ������Ʈ �߰�
	}
}
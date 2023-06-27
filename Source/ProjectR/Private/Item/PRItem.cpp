// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PRItem.h"
#include "Engine/ActorChannel.h"
#include "Component/PRInventoryComponent.h"
#include "Library/PRItemLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APRItem::APRItem()
{
	bReplicates = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(Root);

	Collision = CreateDefaultSubobject<UCapsuleComponent>("Collision");
	Collision->SetCollisionProfileName(FName(TEXT("InteractiveObject")), true);
	Collision->SetupAttachment(Mesh, FName("CollisionSocket"));

}

// Called when the game starts or when spawned
void APRItem::BeginPlay()
{
	Super::BeginPlay();
}

void APRItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APRItem, ItemData);
}

void APRItem::Init(FPRItemData NewItemData)
{
	ItemData = NewItemData;
}

void APRItem::OnRep_ItemData()
{
	const FPRBaseData BaseData = UPRItemLibrary::GetBaseData(ItemData.ID);

	Mesh->SetStaticMesh(BaseData.Mesh);
	Collision->SetCapsuleHalfHeight(BaseData.CollisionHalfHeight);
	Collision->SetCapsuleRadius(BaseData.CollisionRadius);
}


/*
void APRItem::Init(UPRItemDataObject* NewItemDataObject)
{
	//ItemDataObject = NewItemDataObject;
	//ItemDataObject->Rename(nullptr, this);
}
*/

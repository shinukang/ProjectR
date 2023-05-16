// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PRItem.h"

#include "Component/PRInventoryComponent.h"
#include "Library/PRItemStructLibrary.h"
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
	DOREPLIFETIME(APRItem, ItemObject);
}

void APRItem::Init()
{
	ItemObject = NewObject<UPRItemObject>(this);

	if(ItemObject)
	{
		ItemObject->Init();

		Mesh->SetStaticMesh(ItemObject->BaseData.Mesh);
		Mesh->SetRelativeTransform(ItemObject->BaseData.MeshTransform);
		Mesh->SetCollisionProfileName(FName(TEXT("NoCollision")));

		Collision->SetCapsuleHalfHeight(ItemObject->BaseData.CollisionHalfHeight);
		Collision->SetCapsuleRadius(ItemObject->BaseData.CollisionRadius);
	}
}

// Called every frame
void APRItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool APRItem::CanInteract_Implementation()
{
	return bIsInteractive;
}

FName APRItem::GetItemID_Implementation()
{
	return ID;
}

int32 APRItem::GetItemAmount_Implementation()
{
	return 1;
}

void APRItem::OnInteract_Implementation(UPRInventoryComponent* InventoryComponent)
{
	//ItemObject->Rename(TEXT("%s"), InventoryComponent);
	//ItemObject = nullptr;
	//Destroy();
}

void APRItem::DestroyItem()
{
	if(GetOwner()->HasAuthority())
	{
		Server_DestroyItem();
	}
}

void APRItem::Server_DestroyItem_Implementation()
{
	Destroy();
}





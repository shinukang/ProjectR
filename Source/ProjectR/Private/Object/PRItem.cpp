// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/PRItem.h"

#include "Component/PRInventoryComponent.h"
#include "Library/PRItemStructLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APRItem::APRItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Mesh = CreateDefaultSubobject<UMeshComponent>("Body");
	RootComponent = Mesh;

	Collision = CreateDefaultSubobject<UCapsuleComponent>("Collision");
	Collision->SetRelativeRotation(FRotator( 0.0f, 0.0f, 90.0f));
	Collision->SetCollisionProfileName(FName(TEXT("InteractiveObject")), true);
}

// Called when the game starts or when spawned
void APRItem::BeginPlay()
{
	Super::BeginPlay();
}

void APRItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APRItem, ID);
}

void APRItem::Init()
{
	if (const UDataTable* MasterDataTable = URyanLibrary::GetDataTable())
	{
		if (const FPRItemData* ObjectData = MasterDataTable->FindRow<FPRItemData>(ID, TEXT("MasterDataTable")))
		{
			Mesh->SetCollisionProfileName(FName(TEXT("NoCollision")), true);
			Collision->SetCapsuleHalfHeight(ObjectData->CollisionHalfHeight);
			Collision->SetCapsuleRadius(ObjectData->CollisionRadius);
		}
	}
}

void APRItem::OnRep_ID()
{
	
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
	return Amount;
}

void APRItem::OnInteract_Implementation(APlayerController* PlayerController)
{
	UE_LOG(LogTemp, Warning, TEXT("%s try interact to %s"), *PlayerController->GetName(), *GetName());

	if(UPRInventoryComponent* InventoryComponent = Cast<UPRInventoryComponent>(PlayerController->GetComponentByClass(UPRInventoryComponent::StaticClass())))
	{
		InventoryComponent->TryAddToInventory(ID, Amount);
	}
}



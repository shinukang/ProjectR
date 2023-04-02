// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/PRAmmunition.h"

#include "Library/PRItemStructLibrary.h"

APRAmmunition::APRAmmunition()
{
	Type = EPRItemType::Ammunition;
}

void APRAmmunition::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void APRAmmunition::BeginPlay()
{
	Super::BeginPlay();
}

void APRAmmunition::Init()
{
	Super::Init();

	if (const UDataTable* SubDataTable = URyanLibrary::GetDataTable(Type))
	{
		if (const FPRAmmunitionData* AmmunitionData = SubDataTable->FindRow<FPRAmmunitionData>(ID, TEXT("SubDataTable")))
		{
			Cast<UStaticMeshComponent>(Mesh)->SetStaticMesh(AmmunitionData->BodyMesh);
		}
	}
}

void APRAmmunition::OnRep_ID()
{
	
}


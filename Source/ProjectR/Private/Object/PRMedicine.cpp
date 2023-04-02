// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/PRMedicine.h"
#include "Library/PRItemStructLibrary.h"

APRMedicine::APRMedicine()
{
	Type = EPRItemType::Medicine;
}

void APRMedicine::BeginPlay()
{
	Super::BeginPlay();
}

void APRMedicine::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void APRMedicine::Init()
{
	Super::Init();

	if (const UDataTable* SubDataTable = URyanLibrary::GetDataTable(Type))
	{
		if (const FPRMedicineData* MedicineData = SubDataTable->FindRow<FPRMedicineData>(ID, TEXT("SubDataTable")))
		{
			Cast<UStaticMeshComponent>(Mesh)->SetStaticMesh(MedicineData->BodyMesh);
		}
	}
}

void APRMedicine::OnRep_ID()
{
	
}


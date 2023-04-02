// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/PRFirearm.h"

#include "Library/PRItemStructLibrary.h"
#include "Library/RyanLibrary.h"

APRFirearm::APRFirearm()
{
	MagMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mag");
	MagMesh->SetupAttachment(Mesh, FName(TEXT("mag")));

	ScopeMesh = CreateDefaultSubobject<UStaticMeshComponent>("Scope");
	ScopeMesh->SetupAttachment(Mesh, FName(TEXT("scope")));

	Type = EPRItemType::Firearm;
}

void APRFirearm::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void APRFirearm::BeginPlay()
{
	Super::BeginPlay();
}

void APRFirearm::OnRep_ID()
{

}

void APRFirearm::Init()
{
	Super::Init();

	if(const UDataTable* SubDataTable = URyanLibrary::GetDataTable(Type))
	{
		if (const FPRFirearmData* FirearmData = SubDataTable->FindRow<FPRFirearmData>(ID, TEXT("SubDataTable")))
		{
			Cast<USkeletalMeshComponent>(Mesh)->SetSkeletalMeshAsset(FirearmData->BodyMesh);
			MagMesh->SetStaticMesh(FirearmData->MagMesh);
			ScopeMesh->SetStaticMesh(FirearmData->ScopeMesh);
		}
	}
}

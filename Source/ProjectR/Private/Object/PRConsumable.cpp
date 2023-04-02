// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/PRConsumable.h"

APRConsumable::APRConsumable()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	Collision->SetupAttachment(Mesh, FName(TEXT("CollisionSocket")));
}

void APRConsumable::BeginPlay()
{
	Super::BeginPlay();
}

void APRConsumable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void APRConsumable::Init()
{
	Super::Init();
}

void APRConsumable::OnRep_ID()
{
	
}



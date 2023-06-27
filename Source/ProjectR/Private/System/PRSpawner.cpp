// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PRSpawner.h"
#include "Item/PRItem.h"

// Sets default values
APRSpawner::APRSpawner()
{
	bReplicates = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}

// Called when the game starts or when spawned
void APRSpawner::BeginPlay()
{
	Super::BeginPlay();
}



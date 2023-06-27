// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PRLiveCharacterSpawnPoint.h"

// Sets default values
APRLiveCharacterSpawnPoint::APRLiveCharacterSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}

// Called when the game starts or when spawned
void APRLiveCharacterSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APRLiveCharacterSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


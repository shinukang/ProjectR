// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/PRBaseObject.h"

// Sets default values
APRBaseObject::APRBaseObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APRBaseObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APRBaseObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PRGameMode.h"

#include "Item/PRItem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "System/PRPlayerState.h"
#include "System/PRSpawner.h"

void APRGameMode::BeginPlay()
{
	Server_InitItems();
}

void APRGameMode::Server_InitItems_Implementation()
{
	TArray<AActor*> Actors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APRItem::StaticClass(), Actors);

	for(AActor* Actor : Actors)
	{
		if (APRItem* PRItem = Cast<APRItem>(Actor))
		{
			const FString NameStr = FString::Printf(TEXT("%s_DataObject"), *PRItem->GetName());
			PRItem->Init();
		}
	}
}

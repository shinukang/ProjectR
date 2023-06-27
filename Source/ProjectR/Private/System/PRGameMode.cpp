// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PRGameMode.h"

#include "Item/PRItem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "System/PRPlayerState.h"
#include "System/PRSpawner.h"

void APRGameMode::BeginPlay()
{
	Server_SpawnItems();
}

void APRGameMode::OnPostLogin(AController* NewPlayer)
{
	/*
	UE_LOG(LogTemp, Warning, TEXT("APRGameMode::OnPostLogin : NewPlayer = %s, ID = %d"), *NewPlayer->GetPlayerState<APlayerState>()->GetPlayerName(), NewPlayer->GetPlayerState<APlayerState>()->GetPlayerId());

	if (APRPlayerState* PRPlayerState = NewPlayer->GetPlayerState<APRPlayerState>())
	{
		PRPlayerState->LoadCostume();
	}
	else
	{
		if (APlayerState* NewPlayerState = NewPlayer->GetPlayerState<APlayerState>())
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerState is valid. %s"), *NewPlayerState->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerState is invalid."));
		}
	}
	*/
}

void APRGameMode::Server_SpawnItems_Implementation()
{
	TArray<AActor*> Actors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APRSpawner::StaticClass(), Actors);

	for(AActor* Actor : Actors)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawner : %s"), *Actor->GetName());

		if(APRSpawner* Spawner = Cast<APRSpawner>(Actor))
		{
			int Count = 1;

			FVector SpawnLocation = Actor->GetActorLocation();
			FRotator SpawnRotation = FRotator::ZeroRotator;

			for(FPRItemSpawnSetting ItemSpawnSetting : Spawner->SpawnSettings)
			{
				const int32 DistanceRate = Count / 4 + 1;

				switch (Count % 4)
				{
				case 0:
					SpawnLocation += FVector(DistanceRate * Spawner->DistanceBetweenItem, DistanceRate * Spawner->DistanceBetweenItem, 0.0f);
					break;
				case 1:
					SpawnLocation += FVector(DistanceRate * Spawner->DistanceBetweenItem, -DistanceRate * Spawner->DistanceBetweenItem, 0.0f);
					break;
				case 2:
					SpawnLocation += FVector(-DistanceRate * Spawner->DistanceBetweenItem, DistanceRate * Spawner->DistanceBetweenItem, 0.0f);
					break;
				case 3:
					SpawnLocation += FVector(-DistanceRate * Spawner->DistanceBetweenItem, -DistanceRate * Spawner->DistanceBetweenItem, 0.0f);
					break;
				default:
					break;
				}

				if (APRItem* NewItem = GetWorld()->SpawnActor<APRItem>(SpawnLocation, SpawnRotation))
				{
					const FString NameStr = FString::Printf(TEXT("%s_DataObject"), *NewItem->GetName());
					NewItem->Init(FPRItemData(ItemSpawnSetting.ID, ItemSpawnSetting.Amount));
				}
				Count++;
			}
		}
	}
}


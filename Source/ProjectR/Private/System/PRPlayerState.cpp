// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PRPlayerState.h"

#include "Character/PRCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Library/PRCharacterStructLibrary.h"
#include "System/PRLiveCharacter.h"
#include "System/PRLiveCharacter.h"
#include "System/PRSaveGame.h"

APRPlayerState::APRPlayerState()
{


}

void APRPlayerState::SetPlayerName(const FString& NewPlayerName)
{
	Super::SetPlayerName(NewPlayerName);

	UE_LOG(LogTemp, Warning, TEXT("APRPlayerState::SetPlayerName : Name = %s"), *GetPlayerName());
}

void APRPlayerState::SaveCostume()
{
	if(UPRSaveGame* PRSaveGame = NewObject<UPRSaveGame>())
	{
		PRSaveGame->Costumes = Costumes;

		if (UGameplayStatics::SaveGameToSlot(PRSaveGame, GetPlayerName(), 0))
		{
			UE_LOG(LogClass, Warning, TEXT("Success To Save Game."));
		}
		else
		{
			UE_LOG(LogClass, Warning, TEXT("Failure To Save Game."));
		}
	}
}

void APRPlayerState::LoadCostume()
{
	UE_LOG(LogTemp, Warning, TEXT("APRPlayerState::LoadCostume"));

	if(UPRSaveGame* PRSaveGame = Cast<UPRSaveGame>(UGameplayStatics::LoadGameFromSlot(GetPlayerName(), 0)))
	{
		Costumes = PRSaveGame->Costumes;

		for(FPRCostume Costume : Costumes)
		{
			UE_LOG(LogTemp, Warning, TEXT("APRPlayerState::LoadCostume : %s"), *Costume.Name.ToString());
		}
		ApplyCostume();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("APRPlayerState::LoadCostume : Can't load save data"));
	}
	
}

void APRPlayerState::UpdateCostume(FPRCostume Costume)
{
	int32 Index = INDEX_NONE;

	if(Costumes.Find(Costume, Index))
	{
		Costumes.RemoveAt(Index);
	}
	Costumes.AddUnique(Costume);
	ApplyCostume();
}

void APRPlayerState::ApplyCostume()
{
	if(APRLiveCharacter* LobbyPawn = Cast<APRLiveCharacter>(GetPawn()))
	{
		UE_LOG(LogClass, Warning, TEXT("APRPlayerState::ApplyCosutme : LobbyPawn."));
		LobbyPawn->Client_UpdateCostume(Costumes);
	}

	if(APRCharacter* Character = Cast<APRCharacter>(GetPawn()))
	{
		UE_LOG(LogClass, Warning, TEXT("APRPlayerState::ApplyCosutme : Character."));
		Character->Multicast_UpdateCostume(Costumes);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PRCostumeObject.h"

#include "Kismet/GameplayStatics.h"
#include "System/PRGameInstance.h"
#include "System/PRPlayerState.h"

void UPRCostumeObject::OnSelect()
{
	if(UPRGameInstance* GameInstance = Cast<UPRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		//GameInstance->SetCharacterCostume(CostumeData);
	}

	if(APRPlayerState* PlayerState = Cast<APRPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0)))
	{
		PlayerState->UpdateCostume(CostumeData);
	}
}

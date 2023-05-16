// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PRCostumeObject.h"

#include "Kismet/GameplayStatics.h"
#include "System/PRGameInstance.h"

void UPRCostumeObject::OnSelect()
{
	if(UPRGameInstance* GameInstance = Cast<UPRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GameInstance->SetCharacterCostume(CostumeData);
	}
}

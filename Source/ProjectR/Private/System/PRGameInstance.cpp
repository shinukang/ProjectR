// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PRGameInstance.h"

void UPRGameInstance::SetCharacterCostume(FPRCostume CostumeData)
{
	for (int32 Index = 0; Index < CharacterCostumeSetting.Num(); Index++)
	{
		if (CharacterCostumeSetting[Index].Category == CostumeData.Category)
		{
			CharacterCostumeSetting[Index] = CostumeData;
			return;
		}
	}
	CharacterCostumeSetting.Add(CostumeData);
}


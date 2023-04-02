// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/RyanLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Engine/DataTable.h"
#include "Library/PRItemStructLibrary.h"

void URyanLibrary::SetupInputs(UObject* Object, APlayerController* PlayerController, UInputMappingContext* InputMappingContext, bool bNeedToClearMappings)
{
	if(InputMappingContext)
	{
		const TArray<FEnhancedActionKeyMapping>& Mappings = InputMappingContext->GetMappings();
		if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			if (bNeedToClearMappings)
			{
				EnhancedInputComponent->ClearActionEventBindings();
				EnhancedInputComponent->ClearActionValueBindings();
				EnhancedInputComponent->ClearDebugKeyBindings();
			}

			TSet<const UInputAction*> UniqueActions;
			for (const FEnhancedActionKeyMapping& KeyMapping : Mappings)
			{
				UniqueActions.Add(KeyMapping.Action);
			}
			for (const UInputAction* UniqueAction : UniqueActions)
			{
				EnhancedInputComponent->BindAction(UniqueAction, ETriggerEvent::Triggered, Object, UniqueAction->GetFName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent is null"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InputMappingContext is null"));
	}

	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		FModifyContextOptions Options;
		Options.bIgnoreAllPressedKeysUntilRelease = false;
		Options.bForceImmediately = true;

		if(bNeedToClearMappings)
		{
			Subsystem->ClearAllMappings();
		}
		Subsystem->AddMappingContext(InputMappingContext, 1, Options);
																																		}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LocalPlayerSubsystem is null"));
	}
}

UDataTable* URyanLibrary::GetDataTable(EPRItemType Type)
{
	switch(Type)
	{
	case EPRItemType::Default:
		return Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, ItemTablePath));

	case EPRItemType::Firearm:
		return Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, FirearmTablePath));

	case EPRItemType::Ammunition:
		return Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, AmmunitionTablePath));

	case EPRItemType::Medicine:
		return Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, MedicineTablePath));
	}
	return nullptr;
}
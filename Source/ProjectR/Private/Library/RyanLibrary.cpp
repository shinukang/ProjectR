// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/RyanLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

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
	}

	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		FModifyContextOptions Options;
		Options.bForceImmediately = 1;

		if(bNeedToClearMappings)
		{
			Subsystem->ClearAllMappings();
		}
		Subsystem->AddMappingContext(InputMappingContext, 1, Options);
	}
}


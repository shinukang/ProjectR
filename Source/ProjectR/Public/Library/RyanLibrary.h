// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PRCharacterEnumLibrary.h"
#include "PRItemLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RyanLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API URyanLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void SetupInputs(UObject* Object, APlayerController* PlayerController, UInputMappingContext* InputMappingContext, bool bNeedToClearMappings=false);
};

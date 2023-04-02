// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Library/PRItemEnumLibrary.h"
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

	UFUNCTION(BlueprintCallable)
	static UDataTable* GetDataTable(EPRItemType ItemType = EPRItemType::Default);

private:
	const static inline TCHAR* ItemTablePath = TEXT("DataTable'/Game/Data/DT_Item.DT_Item'");

	const static inline TCHAR* FirearmTablePath = TEXT("DataTable'/Game/Data/DT_Firearm.DT_Firearm'");

	const static inline TCHAR* AmmunitionTablePath = TEXT("DataTable'/Game/Data/DT_Ammunition.DT_Ammunition'");

	const static inline TCHAR* MedicineTablePath = TEXT("DataTable'/Game/Data/DT_Medicine.DT_Medicine'");
};

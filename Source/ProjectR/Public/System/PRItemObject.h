// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Library/PRItemStructLibrary.h"
#include "UObject/NoExportTypes.h"
#include "Library/PRWidgetEnumLibrary.h"
#include "PRItemObject.generated.h"

/**
 *
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTR_API UPRItemObject : public UDragDropOperation
{
	GENERATED_BODY()

public:
	void Init();

	void Init(int32 Amount);

	UFUNCTION(BlueprintCallable)
	void AddItemToInventory(APlayerController* PlayerController, int32 Index = -1);

	template<class T>
	T* GetAdvancedData()
	{
		if (ID == NAME_None)
		{
			UE_LOG(LogTemp, Warning, TEXT("ID is NAME_None"));
			return nullptr;
		}
		if(MainCategory == EPRMainCategory::Default)
		{
			UE_LOG(LogTemp, Warning, TEXT("Category is invalid"));
			return nullptr;
		}
		if(!T::StaticStruct()->IsChildOf(FPRAdvancedData::StaticStruct()))
		{
			UE_LOG(LogTemp, Warning, TEXT("T is not child of FPRAdvancedData"));
			return nullptr;
		}

		if(const UDataTable* DataTable = URyanLibrary::GetItemDataTable(MainCategory))
		{
			return DataTable->FindRow<T>(ID, TEXT(""));
		}
		UE_LOG(LogTemp, Warning, TEXT("DataTable is nullptr"));
		return nullptr;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	FPRBaseData BaseData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int32 Amount = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FName ID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	EPRMainCategory MainCategory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	EPRSubCategory SubCategory;
};

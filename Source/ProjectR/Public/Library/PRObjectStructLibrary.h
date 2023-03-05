#pragma once
#include "PRObjectEnumLibrary.h"
#include "Components/Image.h"
#include "Engine/DataTable.h"

#include "PRObjectStructLibrary.generated.h"

USTRUCT()
struct FPRMaster : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Path")
	FString TablePath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Description")
	FString Description = FString(TEXT("Description for Sub-Table"));
};

USTRUCT(BlueprintType)
struct FPRObject : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DisplayName")
	FText ObjectName = FText::FromString(TEXT("None"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Icon")
	TObjectPtr<UImage> Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Type")
	EPRObjectType ObjectType = EPRObjectType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	bool bIsStackable = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Description")
	FString Description = FString(TEXT("Description for Object"));

	FPRObject()
	{
		
	}

	FPRObject(FPRObject* StructRef)
	{
		this->ObjectName = StructRef->ObjectName;
		this->Icon = StructRef->Icon;
		this->Description = StructRef->Description;
	}
};
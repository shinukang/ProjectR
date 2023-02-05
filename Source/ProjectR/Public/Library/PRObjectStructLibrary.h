#pragma once
#include "Engine/DataTable.h"

#include "PRObjectStructLibrary.generated.h"

USTRUCT(BlueprintType)
struct FPRObject : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "SubDataTable")
	TObjectPtr<UDataTable> SubDataTable = nullptr;

	UPROPERTY(EditAnywhere, Category = "Description")
	FString Description = FString(TEXT("Description for DataTable"));
};

USTRUCT(BlueprintType)
struct FPRRifle : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Name")
	FName RifleName;
};
#pragma once
#include "Components/Image.h"
#include "Engine/DataTable.h"

#include "PRObjectStructLibrary.generated.h"

USTRUCT(BlueprintType)
struct FPRObject : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DisplayName")
	FText ObjectName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Icon")
	TObjectPtr<UImage> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Description")
	FString Description = FString(TEXT("Description for DataTable"));
};
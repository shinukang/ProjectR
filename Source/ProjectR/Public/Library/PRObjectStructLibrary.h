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
	FText Name = FText::FromString(TEXT("None"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Icon")
	TObjectPtr<UImage> Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Type")
	EPRObjectType Type = EPRObjectType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	bool bIsStackable = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Description")
	FString Description = FString(TEXT("Description for Object"));

	FPRObject()
	{
		
	}

	FPRObject(FPRObject* StructRef)
	{
		this->Name = StructRef->Name;
		this->Icon = StructRef->Icon;
		this->Type = StructRef->Type;
		this->bIsStackable = StructRef->bIsStackable;
		this->Description = StructRef->Description;
	}

	bool operator==(const FPRObject& Other) const
	{
		return Equals(Other);
	}

	bool Equals(const FPRObject& Other) const
	{
		return this->Name.EqualTo(Other.Name);
	}
};

FORCEINLINE uint32 GetTypeHash(const FPRObject& ObjectData)
{
	uint32 Hash = FCrc::MemCrc32(&ObjectData, sizeof(FPRObject));
	return Hash;
}

USTRUCT(BlueprintType)
struct FPRInventorySlotData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotData")
	FName ID = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotData")
	int32 Amount = 0;

	FPRInventorySlotData()
	{
		
	}

	FPRInventorySlotData(FName ID, int32 Amount)
	{
		this->ID = ID;
		this->Amount = Amount;
	}

	bool operator==(const FPRInventorySlotData& Other) const
	{
		return this->ID.IsEqual(Other.ID, ENameCase::CaseSensitive) ;
	}
};
#pragma once
#include "PRItemEnumLibrary.h"
#include "Components/Image.h"
#include "Engine/DataTable.h"
#include "Object/PRItem.h"

#include "PRItemStructLibrary.generated.h"

USTRUCT()
struct FPRMaster : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Type")
	TSubclassOf<APRItem> ObjectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DataTable")
	TObjectPtr<UDataTable> DataTable = nullptr;
};

USTRUCT(BlueprintType)
struct FPRItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DisplayName")
	FText Name = FText::FromString(TEXT("None"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Icon")
	TObjectPtr<UImage> Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float WeightPerPiece = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	float CollisionHalfHeight = 44.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	float CollisionRadius = 22.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Type")
	EPRItemType Type = EPRItemType::Default;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Description")
	FString Description = FString(TEXT("Description for Object"));

	FPRItemData()
	{
		
	}

	FPRItemData(FPRItemData* StructRef)
	{
		this->Name = StructRef->Name;
		this->Icon = StructRef->Icon;
		this->WeightPerPiece = StructRef->WeightPerPiece;
		this->CollisionHalfHeight = StructRef->CollisionHalfHeight;
		this->CollisionRadius = StructRef->CollisionRadius;
		this->Type = StructRef->Type;
		this->Description = StructRef->Description;
	}

	bool operator==(const FPRItemData& Other) const
	{
		return Equals(Other);
	}

	bool Equals(const FPRItemData& Other) const
	{
		return this->Name.EqualTo(Other.Name);
	}
};

FORCEINLINE uint32 GetTypeHash(const FPRItemData& ObjectData)
{
	uint32 Hash = FCrc::MemCrc32(&ObjectData, sizeof(FPRItemData));
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

USTRUCT(BlueprintType)
struct FPRFirearmData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<USkeletalMesh> BodyMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMesh> MagMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMesh> ScopeMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Type")
	TArray<EPRFireMode> FireModes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spec")
	float Damage = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spec")
	int32 AmmoPerMag = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spec")
	float FireRate = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spec")
	float RecoilRate = 1.0f;
};

USTRUCT(BlueprintType)
struct FPRAmmunitionData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMesh> BodyMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spec")
	EPRBulletType BulletType = EPRBulletType::Bullet_5_56mm;
};

USTRUCT(BlueprintType)
struct FPRMedicineData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMesh> BodyMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spec")
	float RecoveryAmount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spec")
	float RecoveryTime = 0.0f;
};
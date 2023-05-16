#pragma once
#include "PRItemEnumLibrary.h"
#include "RyanLibrary.h"
#include "Components/Image.h"
#include "Engine/DataTable.h"

#include "PRItemStructLibrary.generated.h"

USTRUCT(BlueprintType)
struct FPRAttachmentSlotSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	EPRSubCategory Category = EPRSubCategory::Default;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	int32 Index = 0;
};

USTRUCT(BlueprintType)
struct FPRBaseData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DisplayName")
	FText Name = FText::FromString(TEXT("None"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Category")
	EPRSubCategory Category = EPRSubCategory::Default;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Icon")
	TObjectPtr<UTexture2D> Icon_Small = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Icon")
	TObjectPtr<UTexture2D> Icon_Large = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float WeightPerPiece = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	float CollisionHalfHeight = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	float CollisionRadius = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMesh> Mesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	FTransform MeshTransform = FTransform();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Description")
	FString Description = FString(TEXT("Description for Object"));

	FPRBaseData()
	{
		
	}

	FPRBaseData(FName ID)
	{
		if (const UDataTable* MasterDataTable = URyanLibrary::GetItemDataTable())
		{
			if (FPRBaseData* StructRef = MasterDataTable->FindRow<FPRBaseData>(ID, TEXT("MasterDataTable")))
			{
				this->Name = StructRef->Name;
				this->Category = StructRef->Category;
				this->Icon_Small = StructRef->Icon_Small;
				this->Icon_Large = StructRef->Icon_Large;
				this->WeightPerPiece = StructRef->WeightPerPiece;
				this->CollisionHalfHeight = StructRef->CollisionHalfHeight;
				this->CollisionRadius = StructRef->CollisionRadius;
				this->Mesh = StructRef->Mesh;
				this->MeshTransform = StructRef->MeshTransform;
				this->Description = StructRef->Description;
			}
		}
	}

	FPRBaseData(FPRBaseData* StructRef)
	{
		this->Name = StructRef->Name;
		this->Category = StructRef->Category;
		this->Icon_Small = StructRef->Icon_Small;
		this->Icon_Large = StructRef->Icon_Large;
		this->WeightPerPiece = StructRef->WeightPerPiece;
		this->CollisionHalfHeight = StructRef->CollisionHalfHeight;
		this->CollisionRadius = StructRef->CollisionRadius;
		this->Mesh = StructRef->Mesh;
		this->MeshTransform = StructRef->MeshTransform;
		this->Description = StructRef->Description;
	}

	bool operator==(const FPRBaseData& Other) const
	{
		return Equals(Other);
	}

	bool Equals(const FPRBaseData& Other) const
	{
		return this->Name.EqualTo(Other.Name);
	}
};

FORCEINLINE uint32 GetTypeHash(const FPRBaseData& ObjectData)
{
	uint32 Hash = FCrc::MemCrc32(&ObjectData, sizeof(FPRBaseData));
	return Hash;
}

USTRUCT(BlueprintType)
struct FPRAdvancedData : public FTableRowBase
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FPRFirearmData : public FPRAdvancedData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<USkeletalMesh> BodyMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMesh> MagMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Type")
	TArray<EPRFireMode> FireModes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float Damage = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	int32 AmmoPerMag = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float FireRate = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float RecoilRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float EffectiveRange = 50000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	TArray<FPRAttachmentSlotSetting> AttachmentSlotSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	FVector HolsterOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	EPROverlayState OverlayState = EPROverlayState::Default;

	FPRFirearmData()
	{

	}

	FPRFirearmData(const FPRFirearmData* StructRef)
	{
		this->BodyMesh = StructRef->BodyMesh;
		this->MagMesh = StructRef->MagMesh;
		this->FireModes = StructRef->FireModes;
		this->Damage = StructRef->Damage;
		this->AmmoPerMag = StructRef->AmmoPerMag;
		this->FireRate = StructRef->FireRate;
		this->RecoilRate = StructRef->RecoilRate;
		this->AttachmentSlotSettings = StructRef->AttachmentSlotSettings;
		this->HolsterOffset = StructRef->HolsterOffset;
		this->OverlayState = StructRef->OverlayState;
	}
	
};

USTRUCT(BlueprintType)
struct FPRAttachmentData : public FPRAdvancedData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMesh> BodyMesh = nullptr;

	FPRAttachmentData()
	{
		
	}

	FPRAttachmentData(const FPRAttachmentData* StructRef)
	{
		this->BodyMesh = StructRef->BodyMesh;
	}
};

USTRUCT(BlueprintType)
struct FPREquipmentData : public FPRAdvancedData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<USkeletalMesh> BodyMesh = nullptr;
};

USTRUCT(BlueprintType)
struct FPRAmmunitionData : public FPRAdvancedData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMesh> BodyMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spec")
	EPRBulletType BulletType = EPRBulletType::Bullet_5_56mm;
};

USTRUCT(BlueprintType)
struct FPRMedicineData : public FPRAdvancedData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMesh> BodyMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spec")
	float RecoveryAmount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spec")
	float RecoveryTime = 0.0f;
};

USTRUCT(BlueprintType)
struct FPRItemData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info")
	FName ID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	int32 Amount = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	EPRMainCategory MainCategory = EPRMainCategory::Default;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	EPRSubCategory SubCategory = EPRSubCategory::Default;

	FPRItemData()
	{
		
	}

	FPRItemData(FName NewID, int32 NewAmount = 1)
	{
		ID = NewID;
		Amount = NewAmount;

		SubCategory = GetBaseData()->Category;

		switch (SubCategory)
		{
		case EPRSubCategory::Attachment_Barrel:
		case EPRSubCategory::Attachment_Grip:
		case EPRSubCategory::Attachment_Scope:
			MainCategory = EPRMainCategory::Attachment;
			break;

		case EPRSubCategory::Equipment_Backpack:
		case EPRSubCategory::Equipment_HeadGear:
		case EPRSubCategory::Equipment_Vest:
			MainCategory = EPRMainCategory::Equipment;
			break;

		case EPRSubCategory::Firearm_Primary:
		case EPRSubCategory::Firearm_Secondary:
			MainCategory = EPRMainCategory::Firearm;
			break;

		case EPRSubCategory::Ammunition_Default:
			MainCategory = EPRMainCategory::Ammunition;
			break;

		case EPRSubCategory::Medicine_Default:
			MainCategory = EPRMainCategory::Medicine;
			break;

		case EPRSubCategory::Default:
			MainCategory = EPRMainCategory::Default;
			break;
		}
	}
	FPRBaseData* GetBaseData()
	{
		if(ID == NAME_None)
		{
			UE_LOG(LogTemp, Warning, TEXT("ID is NAME_None"));
			return nullptr;
		}

		if(const UDataTable* DataTable = URyanLibrary::GetItemDataTable())
		{
			return DataTable->FindRow<FPRBaseData>(ID, TEXT(""));
		}
		UE_LOG(LogTemp, Warning, TEXT("DataTable is invalid"));
		return nullptr;
	}

	template<class T>
	T* GetAdvancedData()
	{
		if (ID == NAME_None)
		{
			UE_LOG(LogTemp, Warning, TEXT("ID is NAME_None"));
			return nullptr;
		}
		if (MainCategory == EPRMainCategory::Default)
		{
			UE_LOG(LogTemp, Warning, TEXT("Category is invalid"));
			return nullptr;
		}
		if (!T::StaticStruct()->IsChildOf(FPRAdvancedData::StaticStruct()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Template is not child of FPRAdvancedData"));
			return nullptr;
		}

		if (const UDataTable* DataTable = URyanLibrary::GetItemDataTable(MainCategory))
		{
			return DataTable->FindRow<T>(ID, TEXT(""));
		}
		UE_LOG(LogTemp, Warning, TEXT("DataTable is invalid"));
		return nullptr;
	}

	bool operator==(const FPRItemData& Other) const
	{
		return Equals(Other);
	}

	bool Equals(const FPRItemData& Other) const
	{
		return this->ID == Other.ID;
	}

	bool IsValid() const
	{
		return URyanLibrary::IsValidItemID(this->ID);
	}
};


#pragma once

#include "PRCharacterEnumLibrary.h"
#include "Engine/DataTable.h"
#include "PRItemLibrary.generated.h"

/* Enums for Items */

UENUM(BlueprintType)
enum class EPRCategory : uint8
{
	//
	Default,

	//Firearm
	Firearm_Primary,
	Firearm_Secondary,

	//Equipment
	Equipment_HeadGear,
	Equipment_Vest,
	Equipment_Backpack,

	//Attachment
	Attachment_Grip,
	Attachment_Barrel,
	Attachment_Scope,

	// Ammunition
	Ammunition,

	// Medicine
	Medicine_HealthPoint,
	Medicine_Stamina
};

UENUM()
enum class EPRFireMode : uint8
{
	Auto, // 자동
	Semi_Auto, // 반자동
	Burst, // 3점사
	Bolt_Action // 볼트 액선
};

UENUM()
enum class EPRAmmunition : uint8
{
	Ammunition_5_56mm,
	Ammunition_7_62mm,
	Ammunition_9mm,
	Ammunition_12Gauge
};

/* Structures for Items */
USTRUCT(BlueprintType)
struct FPRAttachmentSlotSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	EPRCategory Category = EPRCategory::Default;

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
	EPRCategory Category = EPRCategory::Default;

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

	FPRBaseData(FName ID);

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMesh> BaseScopeMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Type")
	TArray<EPRFireMode> FireModes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float Damage = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	int32 BulletsPerMag = 10;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	FName AmmunitionID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	bool bNeedBaseScope = false;

	FPRFirearmData()
	{

	}

	FPRFirearmData(const FPRFirearmData* StructRef)
	{
		this->BodyMesh = StructRef->BodyMesh;
		this->MagMesh = StructRef->MagMesh;
		this->BaseScopeMesh = StructRef->BaseScopeMesh;
		this->FireModes = StructRef->FireModes;
		this->Damage = StructRef->Damage;
		this->BulletsPerMag = StructRef->BulletsPerMag;
		this->FireRate = StructRef->FireRate;
		this->RecoilRate = StructRef->RecoilRate;
		this->AttachmentSlotSettings = StructRef->AttachmentSlotSettings;
		this->HolsterOffset = StructRef->HolsterOffset;
		this->OverlayState = StructRef->OverlayState;
		this->AmmunitionID = StructRef->AmmunitionID;
		this->bNeedBaseScope = StructRef->bNeedBaseScope;
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
struct FPRScopeData : public FPRAdvancedData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FOV = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNeedRenderTarget = false;
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
	EPRAmmunition Ammunition = EPRAmmunition::Ammunition_5_56mm;
};

USTRUCT(BlueprintType)
struct FPRMedicineData : public FPRAdvancedData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMesh> BodyMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spec")
	float Efficiency = 0.0f;
};

USTRUCT(BlueprintType)
struct FPRItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	FName ID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	int32 Amount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	EPRCategory Category = EPRCategory::Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	int32 Index = INDEX_NONE;

	FPRItemData()
	{

	}

	FPRItemData(FName NewID, int32 NewAmount = 1);

	FPRItemData(EPRCategory NewCategory)
	{
		Category = NewCategory;
	}

	FPRItemData(FPRItemData* StructRef)
	{
		this->ID = StructRef->ID;
		this->Amount = StructRef->Amount;
		this->Category = StructRef->Category;
		this->Index = StructRef->Index;
	}

	bool operator==(const FPRItemData& Other) const
	{
		return Equals(Other);
	}

	bool Equals(const FPRItemData& Other) const
	{
		return this->ID == Other.ID;
	}
};


/* Item Manage Class */

struct FPRBaseData;

UCLASS(Blueprintable, BlueprintType)
class PROJECTR_API UPRItemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UDataTable* GetItemDataTable(EPRCategory Category);

	UFUNCTION(BlueprintCallable)
	static FPRBaseData GetBaseData(FName ID);

	template<class T>
	static T* GetAdvancedData(FName ID)
	{
		if (IsValidItemID(ID))
		{
			if (const UDataTable* DataTable = GetItemDataTable(GetBaseData(ID).Category))
			{
				const FString ContextStr = FString::Printf(TEXT("ID[%s] is Invalid"), *ID.ToString());
				return DataTable->FindRow<T>(ID, ContextStr);
			}
			return nullptr;
		}
		return nullptr;
	}

	template<class T>
	static T* GetAdvancedData(FName ID, EPRCategory Category)
	{
		if (IsValidItemID(ID))
		{
			if (const UDataTable* DataTable = GetItemDataTable(Category))
			{
				const FString ContextStr = FString::Printf(TEXT("ID[%s] is Invalid"), *ID.ToString());
				return DataTable->FindRow<T>(ID, ContextStr);
			}
			return nullptr;
		}
		return nullptr;
	}

	UFUNCTION(BlueprintCallable)
	static bool IsValidItemID(FName ID);

private:
	const static inline TCHAR* ItemTablePath = TEXT("DataTable'/Game/Data/DT_Item.DT_Item'");

	const static inline TCHAR* FirearmTablePath = TEXT("DataTable'/Game/Data/DT_Firearm.DT_Firearm'");

	const static inline TCHAR* AmmunitionTablePath = TEXT("DataTable'/Game/Data/DT_Ammunition.DT_Ammunition'");

	const static inline TCHAR* MedicineTablePath = TEXT("DataTable'/Game/Data/DT_Medicine.DT_Medicine'");

	const static inline TCHAR* AttachmentTablePath = TEXT("DataTable'/Game/Data/DT_Attachment.DT_Attachment'");

	const static inline TCHAR* EquipmentTablePath = TEXT("DataTable'/Game/Data/DT_Equipment.DT_Equipment'");

	const static inline TCHAR* ScopeTablePath = TEXT("DataTable'/Game/Data/DT_Scope.DT_Scope'");
};


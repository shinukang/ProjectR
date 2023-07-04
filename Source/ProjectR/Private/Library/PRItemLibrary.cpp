#include "Library/PRItemLibrary.h"

FPRBaseData::FPRBaseData(FName ID)
{
	if (const UDataTable* MasterDataTable = UPRItemLibrary::GetItemDataTable(EPRCategory::Default))
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

FPRItemData::FPRItemData(FName NewID, int32 NewAmount)
{
	ID = NewID;
	Amount = NewAmount;
	Category = UPRItemLibrary::GetBaseData(NewID).Category;
}

UDataTable* UPRItemLibrary::GetItemDataTable(EPRCategory Category)
{
	switch (Category)
	{
	case EPRCategory::Default:
		return Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, ItemTablePath));

	case EPRCategory::Firearm_AssaultRifle:
	case EPRCategory::Firearm_Shotgun:
	case EPRCategory::Firearm_SubMachineGun:
	case EPRCategory::Firearm_SniperRifle:
	case EPRCategory::Firearm_Pistol:
		return Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, FirearmTablePath));

	case EPRCategory::Attachment_Barrel:
	case EPRCategory::Attachment_Grip:
		return Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, AttachmentTablePath));

	case EPRCategory::Attachment_Scope:
		return Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, ScopeTablePath));

	case EPRCategory::Equipment_Backpack:
	case EPRCategory::Equipment_HeadGear:
	case EPRCategory::Equipment_Vest:
		return Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, EquipmentTablePath));

	case EPRCategory::Medicine_HealthPoint:
	case EPRCategory::Medicine_Stamina:
		return Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, MedicineTablePath));

	case EPRCategory::Ammunition:
		return Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, AmmunitionTablePath));
	}
	return nullptr;
}

FPRBaseData UPRItemLibrary::GetBaseData(FName ID)
{
	if (IsValidItemID(ID))
	{
		if (const UDataTable* DataTable = GetItemDataTable(EPRCategory::Default))
		{
			return DataTable->FindRow<FPRBaseData>(ID, TEXT("??????"));
		}
		return FPRBaseData();
	}
	return FPRBaseData();
}

bool UPRItemLibrary::IsValidItemID(FName ID)
{
	if (UDataTable* DataTable = GetItemDataTable(EPRCategory::Default))
	{
		TArray<FName> RowNames = DataTable->GetRowNames();

		if (RowNames.Contains(ID))
		{
			return true;
		}
		UE_LOG(LogTemp, Warning, TEXT("UPRItemLibrary::IsValidItemID : [%s] is invalid."), *ID.ToString());
		return false;
	}
	UE_LOG(LogTemp, Warning, TEXT("UPRItemLibrary::IsValidItemID : DataTable is invalid."));
	return false;
}

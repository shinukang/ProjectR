#pragma once
#include "UObject/ObjectMacros.h"

UENUM(BlueprintType)
enum class EPRMainCategory : uint8
{
	Default,
	Firearm,
	Equipment,
	Attachment,
	Ammunition,
	Medicine
};

UENUM(BlueprintType)
enum class EPRSubCategory : uint8
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
	Ammunition_Default,

	// Medicine
	Medicine_Default
};

UENUM()
enum class EPRFireMode : uint8
{
	Auto, // �ڵ�
	Semi_Auto, // ���ڵ�
	Burst, // 3����
	Bolt_Action // ��Ʈ �׼�
};

UENUM()
enum class EPRBulletType : uint8
{
	Bullet_5_56mm,
	Bullet_7_62mm,
	Bullet_9mm,
	Bullet_12Gauge
};
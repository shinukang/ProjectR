#pragma once
#include "UObject/ObjectMacros.h"

UENUM()
enum class EPRItemType : uint8
{
	Default,

	// Weapons
	Firearm,

	// Consumables
	Ammunition,
	Medicine
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
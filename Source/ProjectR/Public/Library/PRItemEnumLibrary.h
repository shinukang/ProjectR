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
	Auto, // 자동
	Semi_Auto, // 반자동
	Burst, // 3점사
	Bolt_Action // 볼트 액선
};

UENUM()
enum class EPRBulletType : uint8
{
	Bullet_5_56mm,
	Bullet_7_62mm,
	Bullet_9mm,
	Bullet_12Gauge
};
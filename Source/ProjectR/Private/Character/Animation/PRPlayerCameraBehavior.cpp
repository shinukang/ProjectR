#include "Character/Animation/PRPlayerCameraBehavior.h"

#include "Character/PRBaseCharacter.h"

void UPRPlayerCameraBehavior::SetRotationMode(EPRRotationMode RotationMode)
{
	bVelocityDirection = RotationMode == EPRRotationMode::VelocityDirection;
	bLookingDirection = RotationMode == EPRRotationMode::LookingDirection;
	bAiming = RotationMode == EPRRotationMode::Aiming;
}

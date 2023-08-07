// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/PRAnimNotifyFirearm.h"
#include "Character/PRCharacter.h"

void UPRAnimNotifyFirearm::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(MeshComp)
	{
		if(APRCharacter* PRCharacter = Cast<APRCharacter>(MeshComp->GetOwner()))
		{
			if (APRFirearm* PRFirearm = PRCharacter->GetCurrentHeldFirearm())
			{
				switch (FirearmState)
				{
				case EPRFirearmState::Equip_Start:
					if (USoundCue* HolsterSound = PRFirearm->FirearmData.SFX_Holster)
					{
						UGameplayStatics::SpawnSoundAttached(HolsterSound, PRFirearm->BodyMesh);
					}
					PRCharacter->SetIsEquipping(true);
					break;

				case EPRFirearmState::Equip_Attach:
					PRCharacter->AttachToHand();
					break;
				case EPRFirearmState::Equip_Detach:
					for (APRFirearm* BackFirearm : PRCharacter->PRInventoryComponent->Firearms)
					{
						if (BackFirearm != PRFirearm)
						{
							PRCharacter->AttachToBack(BackFirearm);
						}
					}
					break;

				case EPRFirearmState::Equip_End:
					PRCharacter->SetIsEquipping(false);
					PRCharacter->SetNeedToResetOverlayState(false);
					break;

				case EPRFirearmState::Reload_Start:
					PRCharacter->AttachToHand(false);
					PRCharacter->SetIsReloading(true);
					break;

				case EPRFirearmState::Reload_Out:
					if(USoundCue* MagOutSound = PRFirearm->FirearmData.SFX_Reload_MagOut)
					{
						UGameplayStatics::SpawnSoundAttached(MagOutSound, PRFirearm->BodyMesh);
					}
					break;

				case EPRFirearmState::Reload_In:
					if (USoundCue* MagInSound = PRFirearm->FirearmData.SFX_Reload_MagIn)
					{
						UGameplayStatics::SpawnSoundAttached(MagInSound, PRFirearm->BodyMesh);
					}
					break;

				case EPRFirearmState::Reload_Cock:
					PRCharacter->GetCurrentHeldFirearm()->Reload();
					if (USoundCue* CockSound = PRFirearm->FirearmData.SFX_Reload_Cock)
					{
						UGameplayStatics::SpawnSoundAttached(CockSound, PRFirearm->BodyMesh);
					}
					break;

				case EPRFirearmState::Reload_End:
					PRCharacter->AttachToHand(true);
					PRCharacter->SetIsReloading(false);
					break;
				}
			}
		}
	}
}

FString UPRAnimNotifyFirearm::GetNotifyName_Implementation() const
{
	FString Name(TEXT("Firearm: "));
	Name.Append(GetEnumerationToString(FirearmState));
	return Name;
}


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
			switch(FirearmState)
			{
			case EPRFirearmState::Equip_Start:
				PRCharacter->SetIsEquipping(true);
				break;

			case EPRFirearmState::Equip_Attach:
				PRCharacter->AttachToHand();
				PRCharacter->SetNeedToResetOverlayState(false);
				break;
			case EPRFirearmState::Equip_Detach:
				for (APRFirearm* Firearm : PRCharacter->PRInventoryComponent->Firearms)
				{
					if (Firearm != PRCharacter->GetCurrentHeldFirearm())
					{
						PRCharacter->AttachToBack(Firearm);
					}
				}
				break;
			case EPRFirearmState::Equip_End:
				PRCharacter->SetIsEquipping(false);
				break;

			case EPRFirearmState::Reload_Start:
				PRCharacter->SetIsReloading(true);
				break;

			case EPRFirearmState::Reload_End:
				PRCharacter->GetCurrentHeldFirearm()->Reload();
				PRCharacter->SetIsReloading(false);
				break;
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

